#include "dissectorip.h"
#include "dissectorudp.h"
#include "dissectortcp.h"

StreamRecorder DissectorIp::streamRecorder;
quint32 DissectorIp::flags = 0;
/*
 * bit
 * 0:    0: uncheck    1: check
 */

DissectorIp::DissectorIp(){}

void DissectorIp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    ip_hdr *header = GetIpHdr(dissRes,info==NULL?true:false);
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    if(info == NULL){
        dissResEth->SetIpVersion(4);
        dissRes->AddHeadersLen(DissectorIp::GetIpHdrLen(header));
        dissResEth->SetIpSrc(header->sourceIP);
        dissResEth->SetIpDst(header->destIP);
        dissResEth->SetIpTotalLen(DissectorIp::GetIpTotalLen(header));
        dissResEth->SetIpHeaderLen(DissectorIp::GetIpHdrLen(header));
        dissResEth->ResetProtocolStackAndPosition("ip",DissectorIp::GetIpHdrLen(header) * 4);

        streamRecorder.Add(dissResEth->GetStrIpSrc(),dissResEth->GetStrDst(),dissResEth->GetNo());
    }else{
        qint32 start = dissRes->GetProStart("ip");
        proTree->AddItem("ip",DissectorIp::MsgIpTop(dissRes),start,dissRes->GetProEnd("ip"));
        proTree->AddItem("ip",DissectorIp::MsgIpVersion(header),&start,0.5,false,ProTree::NEW);
        proTree->AddItem("ip",DissectorIp::MsgIpHdrLen(header),&start,1);  //0.5 + 0.5
        DissectorIp::DealIpDS(header,proTree,&start);
        proTree->AddItem("ip",DissectorIp::MsgIpTotalLen(header),&start,2);
        proTree->AddItem("ip",DissectorIp::MsgIdentification(header),&start,2);
        DissectorIp::DealIpFlags(header,proTree,&start);
        proTree->AddItem("ip",DissectorIp::MsgIpTTL(header),&start,1);
        proTree->AddItem("ip",DissectorIp::MsgIpPType(header),&start,1);
        DissectorIp::DealIpChecksum(header,proTree,&start);
        proTree->AddItem("ip",DissectorIp::MsgIpSrc(dissRes),&start,4);
        proTree->AddItem("ip",DissectorIp::MsgIpDst(dissRes),&start,4);
        proTree->Pop();
    }

    switch (GetIpPType(header)) {
    case 6:
        DissectorTcp::Dissect(dissRes,proTree,info);
        break;
    case 17:
        DissectorUdp::Dissect(dissRes,proTree,info);
        break;
    default:
        break;
    }
}


//Get 方法
ip_hdr* DissectorIp::GetIpHdr(DissRes *dissRes,bool first){
    if(first)
        dissRes->AddToProtocolStackWithSE("ip",sizeof(ip_hdr));
    ip_hdr *ip = (ip_hdr*)(dissRes->GetData() + dissRes->GetProStart("ip"));
    return ip;
}

uchar DissectorIp::GetIpVersion(ip_hdr *header){
    return (header->versionAndHeaderLen & 0xf0) >> 4;
}

uchar DissectorIp::GetIpHdrLen(ip_hdr *header){
    return (header->versionAndHeaderLen & 0x0f);
}

uchar DissectorIp::GetIpService(ip_hdr *header){
    return header->tos;
}

quint16 DissectorIp::GetIpTotalLen(ip_hdr *header){
    return ntohs(header->total_len);
}

quint16 DissectorIp::GetIpIdentification(ip_hdr *header){
    return ntohs(header->ident);
}

quint16 DissectorIp::GetIpFlags(ip_hdr *header){
    return ntohs(header->flagsAndOffest);
}

uchar DissectorIp::GetIpFlagsReserved(ip_hdr *header){
    return (ntohs(header->flagsAndOffest) & 0x8000) >> 15 ;
}

uchar DissectorIp::GetIpFlagsDF(ip_hdr *header){
    return (ntohs(header->flagsAndOffest) & 0x4000) >> 14;
}

uchar DissectorIp::GetIpFlagsMF(ip_hdr *header){
    return (ntohs(header->flagsAndOffest) & 0x2000) >> 13;
}

quint16 DissectorIp::GetIpOffset(ip_hdr *header){
    return (ntohs(header->flagsAndOffest) & 0x1fff);
}

uchar DissectorIp::GetTTL(ip_hdr *header){
    return header->ttl;
}

uchar DissectorIp::GetIpPType(ip_hdr *header){
    return header->protocol;
}

quint16 DissectorIp::GetChecksum(ip_hdr *header){
    return ntohs(header->checksum);
}


//Get 方法，参数直接为DissRes*
quint32 DissectorIp::GetIpSrcAddress(DissRes *dissRes){
    ip_hdr *header = GetIpHdr(dissRes);
    quint32 address;
    memcpy(&address,header->sourceIP,IP_LENS::SOURCEIP);
    return ntohl(address);
}

quint32 DissectorIp::GetIpDstAddress(DissRes *dissRes){
    ip_hdr *header = GetIpHdr(dissRes);
    quint32 address;
    memcpy(&address,header->destIP,IP_LENS::DESTIP);
    return ntohl(address);
}

QString DissectorIp::GetIpStrSrcAddress(DissRes *dissRes){
    ip_hdr *header = GetIpHdr(dissRes);
    return QString::asprintf("%d.%d.%d.%d"
            ,header->sourceIP[0]
            ,header->sourceIP[1]
            ,header->sourceIP[2]
            ,header->sourceIP[3]
            );
}

QString DissectorIp::GetIpStrDstAddress(DissRes *dissRes){
    ip_hdr *header = GetIpHdr(dissRes);
    return QString::asprintf("%d.%d.%d.%d"
            ,header->destIP[0]
            ,header->destIP[1]
            ,header->destIP[2]
            ,header->destIP[3]
            );
}

//Msg方法
QString DissectorIp::MsgIpTop(DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    return QString("Internet Protocol Version 4, Src:") + dissResEth->GetStrIpSrc()
            + " , Dst: " + dissResEth->GetStrIpDst();
}

QString DissectorIp::MsgIpVersion(ip_hdr *header){
    return QString::asprintf("%d%d%d%d .... = version :%d"
                             ,(DissectorIp::GetIpVersion(header)&0x08)>>3
                             ,(DissectorIp::GetIpVersion(header)&0x04)>>2
                             ,(DissectorIp::GetIpVersion(header)&0x02)>>1
                             ,(DissectorIp::GetIpVersion(header)&0x01)>>0
                             ,DissectorIp::GetIpVersion(header)
                             );
}

QString DissectorIp::MsgIpHdrLen(ip_hdr *header){
    return QString::asprintf(".... %d%d%d%d = Header Length : %d bytes (%d)"
                             ,(DissectorIp::GetIpHdrLen(header) & 0x08) >> 3
                             ,(DissectorIp::GetIpHdrLen(header) & 0x04) >> 2
                             ,(DissectorIp::GetIpHdrLen(header) & 0x02) >> 1
                             ,(DissectorIp::GetIpHdrLen(header) & 0x01) >> 0
                             ,(DissectorIp::GetIpHdrLen(header) * 4)
                             ,(DissectorIp::GetIpHdrLen(header))
                             );
}

QString DissectorIp::MsgIpTotalLen(ip_hdr *header){
    return QString::asprintf("Total length: %d",DissectorIp::GetIpTotalLen(header));
}

QString DissectorIp::MsgIdentification(ip_hdr *header){
    quint16 ident = DissectorIp::GetIpIdentification(header);
    return QString::asprintf("Identification : 0x%02x%02x (%d)"
                             ,((uchar*)&ident)[1]
                             ,((uchar*)&ident)[0]
                             ,ident
                             );
}

QString DissectorIp::MsgIpTTL(ip_hdr *header){
    return QString::asprintf("Time to live : %d",DissectorIp::GetTTL(header));
}

QString DissectorIp::MsgIpPType(ip_hdr *header){
    uchar type = DissectorIp::GetIpPType(header);
    QString str;
    switch (type) {
    case 6:
        str = "tcp";
        break;
    case 17:
        str = "udp";
        break;
    default:
        str = "unknow";
    }
    return QString("Protocol : ") + str + QString::asprintf("(%d)",type);
}

QString DissectorIp::MsgIpSrc(DissRes *dissRes){
    return QString("Source :") + ((DissResEth*)dissRes)->GetStrIpSrc();
}

QString DissectorIp::MsgIpDst(DissRes *dissRes){
    return QString("Destination :") + ((DissResEth*)dissRes)->GetStrIpDst();
}

//Deal方法
void DissectorIp::DealIpDS(ip_hdr *header, ProTree *tree,qint32 *start){
    QString str;
    uchar ds = DissectorIp::GetIpService(header);
    str = QString::asprintf("Differentiated Services Field: 0x%02x",ds);
    tree->AddItem("ip",str,*start,*start);

    str = QString::asprintf("%d%d%d%d %d%d.. = Differentiated Services Codepoint : %d"
                            ,(ds&0x80)>>7
                            ,(ds&0x40)>>6
                            ,(ds&0x20)>>5
                            ,(ds&0x10)>>4
                            ,(ds&0x08)>>3
                            ,(ds&0x04)>>2
                            ,(ds&0xfc)>>2
                            );
    tree->AddItem("ip",str,*start,*start,ProTree::NEW);

    str = QString::asprintf(".... ..%d%d = Current Unused : %d"
                            ,(ds&0x02)>>1
                            ,(ds&0x01)>>0
                            ,(ds&0x03)
                            );
    tree->AddItem("ip",str,*start,*start);

    tree->Pop();
    *start += 1;
}

void DissectorIp::DealIpFlags(ip_hdr *header, ProTree *tree, qint32 *start){
    quint16 flags = DissectorIp::GetIpFlags(header);
    QString str;
    if(DissectorIp::GetIpFlagsDF(header) == 1)
        str = " , Don't fragment";
    else
        str = " , More fragment";
    str = QString::asprintf("0x%02x%02x"
                            ,((uchar*)&flags)[1]
                            ,((uchar*)&flags)[0]
                             )
            + str;
    tree->AddItem("ip",str,*start,*start+1);

    str = QString::asprintf("%d... .... .... .... = Reserved bit : "
                            ,DissectorIp::GetIpFlagsReserved(header))
            + (DissectorIp::GetIpFlagsReserved(header) == 1?"Set":"Not Set");
    tree->AddItem("ip",str,*start,*start+1,ProTree::NEW);

    str = QString::asprintf(".%d.. .... .... .... = Don't fragment : "
                            ,DissectorIp::GetIpFlagsDF(header))
            + (DissectorIp::GetIpFlagsDF(header) == 1?"Set":"Not Set");
    tree->AddItem("ip",str,*start,*start+1);

    str = QString::asprintf("..%d. .... .... .... = More fragment : "
                            ,DissectorIp::GetIpFlagsMF(header))
            + (DissectorIp::GetIpFlagsMF(header) == 1?"Set":"Not Set");
    tree->AddItem("ip",str,*start,*start+1);

    str = QString::asprintf("...%d %d%d%d%d %d%d%d%d %d%d%d%d = Fragment offset : %d"
                            ,(flags&0x1000)>>12
                            ,(flags&0x0800)>>11
                            ,(flags&0x0400)>>10
                            ,(flags&0x0200)>>9
                            ,(flags&0x0100)>>8
                            ,(flags&0x0080)>>7
                            ,(flags&0x0040)>>6
                            ,(flags&0x0020)>>5
                            ,(flags&0x0010)>>4
                            ,(flags&0x0008)>>3
                            ,(flags&0x0004)>>2
                            ,(flags&0x0002)>>1
                            ,(flags&0x0001)>>0
                            ,DissectorIp::GetIpOffset(header)
                            );
    tree->AddItem("ip",str,*start,*start+1);

    *start+=2;
    tree->Pop();
}

void DissectorIp::DealIpChecksum(ip_hdr *header, ProTree *tree, qint32 *start){
    if(DissectorIp::FlagGetCheck()){
        quint16 chksum = DissectorIp::GetChecksum(header);
        quint16 calChksum = DissectorIp::GetCalculateCheckSum(header);
        bool res = (chksum == calChksum);
        QString strChksum = QString::asprintf("0x%02x%02x",((uchar*)&chksum)[1],((uchar*)&chksum)[0]);
        QString strCalChksum = QString::asprintf("0x%02x%02x",((uchar*)&calChksum)[1],((uchar*)&calChksum)[0]);

        QString msg = QString("Header checksum: ") + strChksum + (res ? QString(" [correct]"):QString(" [wrong]"));
        tree->AddItem("ip",msg,start,2,false);

        msg = QString("[Header checksum status : ") + (res ? QString("Good"):QString("Bad")) + QString(" ]");
        tree->AddItem("ip",msg,start,2,false);

        msg = QString("[ Calculated checksum : ") + strCalChksum + QString(" ]");
        tree->AddItem("ip",msg,start,2);
    }
}

//Flag 方法
void DissectorIp::FlagSetCheck(uchar option){
    if(option == 0)
        DissectorIp::flags &= 0xfffffffe;
    else
        DissectorIp::flags |= 0x00000001;
}

uchar DissectorIp::FlagGetCheck(){
    return (DissectorIp::flags & 0x00000001) >> 0;
}

//others
void DissectorIp::GetInverseSum(quint16 *sum, quint16 num){
    quint32 res = (quint32)*sum + (quint32)num;
    if(res > 0x0000ffff)
        *sum =  (quint16)(res - 0x00010000 + 1);
    else
        *sum = (quint16)(res);
}

quint16 DissectorIp::GetCalculateCheckSum(ip_hdr *header){
    qint32 ipHdrLen = DissectorIp::GetIpHdrLen(header) * 4;
    qDebug() << "IpHdrLen = " << ipHdrLen;
    quint16 *start = (quint16*)header;
    quint16 res = 0;
    for(qint32 index = 0; index < ipHdrLen/2; index++){
        if(index != 5){
            DissectorIp::GetInverseSum(&res,ntohs(start[index]));
        }
    }
    return ~res;
}
