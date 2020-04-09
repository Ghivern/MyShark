#include "dissectorudp.h"

/*
 * bit 0: validate the udp checksum
 */
quint32 DissectorUdp::flags = 0x1;
StreamRecorder DissectorUdp::streamRecorder;


DissectorUdp::DissectorUdp() {

}


void DissectorUdp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    udp_hdr *header = GetUdpHdr(dissRes,info==NULL?true:false);
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    if(info == NULL){
        dissResEth->SetSrcPort(GetUdpSrcPort(header));
        dissResEth->SetDstPort(GetUdpDstPort(header));

        //添加进流记录器
        GetStreamRecorder().Add(dissResEth->GetStrIpSrc()
                                ,dissResEth->GetStrIpDst()
                                ,dissResEth->GetSrcPort()
                                ,dissResEth->GetDstPort()
                                ,dissResEth->GetNo());
    }else{
        proTree->AddItem("udp",UdpMsgSummery(dissResEth));
        proTree->AddItem("udp",UdpMsgSourcePort(dissResEth),ProTree::level::NEW);
        proTree->AddItem("udp",UdpMsgDestinationPort(dissResEth));
        proTree->AddItem("udp",UdpMsgLength(header));
        UdpDealChecksum(proTree,dissRes);
        proTree->AddItem("udp",UdpMsgStreamIndex(dissResEth));
        proTree->Pop();
    }
}

//Get方法
udp_hdr* DissectorUdp::GetUdpHdr(DissRes *dissRes,bool first){
    if(first)
        dissRes->AddToProtocolStackWithSE("udp",sizeof(udp_hdr));
    udp_hdr *udp = (udp_hdr*)(dissRes->GetData() + dissRes->GetProStart("udp"));
    return udp;
}

ushort DissectorUdp::GetUdpDstPort(udp_hdr *header){
    return ntohs(header->dport);
}

ushort DissectorUdp::GetUdpSrcPort(udp_hdr *header){
    return ntohs(header->sport);
}

ushort DissectorUdp::GetUdpLength(udp_hdr *header){
    return ntohs(header->tot_len);
}

ushort DissectorUdp::GetUdpCheckSum(udp_hdr *header){
    return ntohs(header->check_sum);
}

QString DissectorUdp::GetUdpStrCheckSum(udp_hdr *header){
    ushort checksum = GetUdpCheckSum(header);
    return QString::asprintf("0x%02x%02x"
                             ,((uchar*)&checksum)[1]
                             ,((uchar*)&checksum)[0]
                             );
}

//获取StreamRecorder
StreamRecorder& DissectorUdp::GetStreamRecorder(){
    return streamRecorder;
}


//Get方法，参数直接为DissRes*类型
ushort DissectorUdp::GetUdpDstPort(DissRes *dissRes){
    return GetUdpDstPort(GetUdpHdr(dissRes));
}

ushort DissectorUdp::GetUdpSrcPort(DissRes *dissRes){
    return GetUdpSrcPort(GetUdpHdr(dissRes));
}

//Flag方法
bool DissectorUdp::UdpFlagValidateChecksum(){
    if(DissectorUdp::flags & VALIDATE_CHECKSUM)
        return true;
    else
        return false;
}

//Msg方法
QString DissectorUdp::UdpMsgSummery(DissResEth *dissResEth){
    return QString::asprintf("User Datagram Protocol, Src Port : %ud, Dst Port : %ud"
                             ,dissResEth->GetSrcPort()
                             ,dissResEth->GetDstPort()
                             );
}

QString DissectorUdp::UdpMsgSourcePort(DissResEth *dissResEth){
    return QString::asprintf("Source Port : %ud",dissResEth->GetSrcPort());
}

QString DissectorUdp::UdpMsgDestinationPort(DissResEth *dissResEth){
    return QString::asprintf("Destination Port : %ud",dissResEth->GetDstPort());
}

QString DissectorUdp::UdpMsgLength(udp_hdr *header){
    return QString::asprintf("Length : %ud",GetUdpLength(header));
}

void DissectorUdp::UdpDealChecksum(ProTree *tree,DissRes *dissRes){
    QString msg;
    udp_hdr *header = GetUdpHdr(dissRes);
    if(!UdpFlagValidateChecksum()){  //不验证checksum
        msg.append("Checksum : ").append(GetUdpStrCheckSum(header)).append(" [unverified]");
        tree->AddItem("udp",msg);
        msg.clear();
        msg.append("[Check status : unverified]");
        tree->AddItem("udp",msg);
    }else{
        quint16 cal_checksum = GetCalculateCheckSum(dissRes);
        if(cal_checksum == GetUdpCheckSum(header)){  //相等
            msg.append("Checksum : ").append(GetUdpStrCheckSum(header)).append("[correct]");
            tree->AddItem("udp",msg);

            msg.clear();
            msg.append("[Calculated checksum : ")
                    .append(GetStrCalculateCheckSum(cal_checksum))
                    .append("]");
            tree->AddItem("udp",msg,ProTree::level::NEW);
            tree->Pop();

            msg.clear();
            msg.append("[Check status : Good]");
            tree->AddItem("udp",msg,ProTree::level::NEW);
        }else{
            msg.append("Checksum : ").append(GetUdpStrCheckSum(header))
                    .append(" incorrect, should be ")
                    .append(GetStrCalculateCheckSum(cal_checksum))
                    .append( QString::asprintf(" (maybe caused by \"UDP checksum offload\"?") );
            tree->AddItem("udp",msg);

            msg.clear();
            msg.append("[Calculated checksum : ")
                    .append(GetStrCalculateCheckSum(cal_checksum))
                    .append("]");
            tree->AddItem("udp",msg,ProTree::level::NEW);
            tree->Pop();

            msg.clear();
            msg.append("[Checksum status : Bad]");
            tree->AddItem("udp",msg);
        }
    }
}

QString DissectorUdp::UdpMsgStreamIndex(DissResEth *dissResEth){
    qint64 streamIndex =GetStreamRecorder()
            .GetStreamIndex(dissResEth->GetStrSrc(),dissResEth->GetStrDst(),dissResEth->GetSrcPort(),dissResEth->GetDstPort());
    return QString::asprintf("[Stream Index : %lld]",streamIndex);
}

//计算检验和
void DissectorUdp::GetInverseSum(quint16 *sum,quint16 num){
    quint32 res = (quint32)*sum + (quint32)num;
    if(res > 0x0000ffff)
        *sum =  (quint16)(res - 0x00010000 + 1);
    else
        *sum = (quint16)(res);
}

quint16 DissectorUdp::GetCalculateCheckSum(DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    udp_hdr *header = GetUdpHdr(dissRes);
    quint16 udpLen = GetUdpLength(header);
    quint16 res = 0;
    quint32 temp32;
    quint16* start =(quint16*)&temp32;
    /* 伪首部：  源IP(4)  目的IP(4)  0(1)  17(1)  UDP长度(2)*/
    temp32 = htonl(dissResEth->GetIpSrcAddress());
    for(qint32 index = 0; index < 2; index++)
        GetInverseSum(&res,ntohs(start[index]));
    temp32 = htonl(dissResEth->GetIpDstAddress());
    for(qint32 index = 0; index < 2; index++)
        GetInverseSum(&res,ntohs(start[index]));
    temp32 = htonl((0x00000011 << 16) | GetUdpLength(header));
    for(qint32 index = 0; index < 2; index++)
        GetInverseSum(&res,ntohs(start[index]));
    start = (quint16*)GetUdpHdr(dissRes);
    for(qint32 index = 0; index < (qint32)udpLen/2; index++){
        if(index != 3){
            DissectorUdp::GetInverseSum(&res,ntohs(start[index]));
        }
    }
    if(udpLen%2!=0){
        quint16 temp = ((uchar*)header)[udpLen-1];
        GetInverseSum(&res,htons(temp));
    }
    return ~res;
}

QString DissectorUdp::GetStrCalculateCheckSum(DissRes *dissRes){
    quint16 checksum = GetCalculateCheckSum(dissRes);
    return QString::asprintf("0x%02x%02x"
                             ,((uchar*)&checksum)[1]
                             ,((uchar*)&checksum)[0]
                             );
}

QString DissectorUdp::GetStrCalculateCheckSum(quint16 checksum){
    return QString::asprintf("0x%02x%02x"
                             ,((uchar*)&checksum)[1]
                             ,((uchar*)&checksum)[0]
                             );
}
