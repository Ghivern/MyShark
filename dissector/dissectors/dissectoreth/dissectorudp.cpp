#include "dissectorudp.h"

quint32 DissectorUdp::flags = 0;
TUStream DissectorUdp::stream;

//qint16 DissectorUdp::PSEUDO_HEADER_LEN = 12;

DissectorUdp::DissectorUdp()
{

}


void DissectorUdp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    udp_hdr *header = GetUdpHdr(dissRes,info==NULL?true:false);
    if(info == NULL){
        DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissResEth->SetSrcPort(GetUdpSrcPort(header));
        dissResEth->SetDstPort(GetUdpDstPort(header));
        DissectorUdp::stream.Append(dissResEth->GetStrIpSrc()
                                    ,dissResEth->GetSrcPort()
                                    ,dissResEth->GetStrIpDst()
                                    ,dissResEth->GetDstPort());
        qDebug() << "DissectorUdp : (关注 StreamIndex)" <<
                    DissectorUdp::stream.StreamIndex(dissResEth->GetStrIpSrc()
                                                     ,dissResEth->GetSrcPort()
                                                     ,dissResEth->GetStrIpDst()
                                                     ,dissResEth->GetDstPort());
    }else{
        proTree->AddItem("udp","调试");
        proTree->AddItem("udp",DissectorUdp::GetUdpStrCheckSum(header),ProTree::level::NEW);
        proTree->AddItem("udp",DissectorUdp::GetStrCalculateCheckSum(dissRes));
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


//Get方法，参数直接为DissRes*类型
ushort DissectorUdp::GetUdpDstPort(DissRes *dissRes){
    return GetUdpDstPort(GetUdpHdr(dissRes));
}

ushort DissectorUdp::GetUdpSrcPort(DissRes *dissRes){
    return GetUdpSrcPort(GetUdpHdr(dissRes));
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
