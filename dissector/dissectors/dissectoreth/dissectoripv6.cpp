#include "../../units/bit.h"

#include "dissectoripv6.h"
#include "dissectortcp.h"
#include "dissectorudp.h"


quint32 DissectorIpv6::flags = 0;
StreamRecorder DissectorIpv6::streamRecorder;

DissectorIpv6::DissectorIpv6()
{

}

void DissectorIpv6::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    ipv6_hdr *header = Ipv6GetHdr(dissRes,info == NULL?true:false);
    DissResEth *dissResEth = (DissResEth*)dissRes;
    if(info == NULL){
        dissResEth->SetIpVersion(6);
        dissResEth->SetIpv6Src(header->srcAddress);
        dissResEth->SetIpv6Dst(header->dstAddress);
        dissResEth->SetIpTotalLen(Ipv6GetPayloadLength(header) + IPV6_LEN::BASE_HEADER);

        streamRecorder.Add(dissResEth->GetStrIpSrc(),dissResEth->GetStrIpDst(),0,0,dissResEth->GetNo());
    }else{
        qint32 start = dissRes->GetProStart("ipv6");
        proTree->AddItem("ipv6",Ipv6MsgSummery(header,dissResEth),start,dissResEth->GetProEnd("ipv6")); // -
        proTree->AddItem("ipv6",Ipv6MsgVersion(header),&start,IPV6_LEN::VTS,false,ProTree::level::NEW); // - -
        Ipv6DealTrafficClass(header,proTree,&start); // - -
        proTree->AddItem("ipv6",Ipv6MsgFlowLabel(header),&start,IPV6_LEN::VTS); // - -
        proTree->AddItem("ipv6",Ipv6MsgPayloadLength(header),&start,IPV6_LEN::PAYLOAD_LENGTH); //- -
        proTree->AddItem("ipv6",Ipv6MsgNextHeader(header),&start,IPV6_LEN::NEXT_HEADER); // - -
        proTree->AddItem("ipv6",Ipv6MsgHopLimit(header),&start,IPV6_LEN::HOP_LIMIT); // - -
        proTree->AddItem("ipv6",Ipv6MsgSourceAddress(dissResEth),&start,IPV6_LEN::SRC_ADDRESS); // - -
        proTree->AddItem("ipv6",Ipv6MsgDestinationAddress(dissResEth),&start,IPV6_LEN::DST_ADDRESS); // - -
    }

    switch (Ipv6GetNextHeader(header)) {
        case 0:
        case 43:
        case 5:
        case 51:
        case 50:
        case 60:
        case 59:
            break;
        // ... 运输层协议
        case 6:
            DissectorTcp::Dissect(dissRes,proTree,info);
            break;
        case 17:
            DissectorUdp::Dissect(dissRes,proTree,info);
        break;
        case 58:  //icmpv6
        break;
    default:
        qDebug() << "ipv6 dissector: unknow next header";

    }
}

//Get方法
ipv6_hdr* DissectorIpv6::Ipv6GetHdr(DissRes *dissRes, bool first){
    if(first)
        dissRes->AddToProtocolStackWithSE("ipv6",sizeof(ipv6_hdr));
    ipv6_hdr *ipv6 = (ipv6_hdr*)(dissRes->GetData() + dissRes->GetProStart("ipv6"));
    return ipv6;
}

StreamRecorder& DissectorIpv6::Ipv6GetStreamRecorder(){
    return streamRecorder;
}

quint8 DissectorIpv6::Ipv6GetVersion(ipv6_hdr *header){
    quint32 vtf = ntohl(header->vtf);
    return vtf >> (IPV6_LEN::TRAFFIC_CLASS_BIT + IPV6_LEN::FLOW_LABEL_BIT);
}

quint8 DissectorIpv6::Ipv6GetVersion(DissRes *dissRes){
    return Ipv6GetVersion(Ipv6GetHdr(dissRes));
}

quint8 DissectorIpv6::Ipv6GetTrafficClass(ipv6_hdr *header){
    quint32 vtf = ntohl(header->vtf);
    return vtf >> IPV6_LEN::FLOW_LABEL_BIT;

}

quint8 DissectorIpv6::Ipv6GetTrafficClass(DissRes *dissRes){
    return Ipv6GetTrafficClass(Ipv6GetHdr(dissRes));
}

quint32 DissectorIpv6::Ipv6GetFlowLabel(ipv6_hdr *header){
    quint32 vtf = ntohl(header->vtf);
    return vtf & 0x000fffff;
}

quint32 DissectorIpv6::Ipv6GetFlowLabel(DissRes *dissRes){
    return Ipv6GetFlowLabel(Ipv6GetHdr(dissRes));
}

quint16 DissectorIpv6::Ipv6GetPayloadLength(ipv6_hdr *header){
    return ntohs(header->payloadLength);
}

quint16 DissectorIpv6::Ipv6GetPayloadLength(DissRes *dissRes){
    return Ipv6GetPayloadLength(Ipv6GetHdr(dissRes));
}

quint8 DissectorIpv6::Ipv6GetNextHeader(ipv6_hdr *header){
    return header->nextHeader;
}

quint8 DissectorIpv6::Ipv6GetNextHeader(DissRes *dissRes){
    return Ipv6GetNextHeader(Ipv6GetHdr(dissRes));
}

quint8 DissectorIpv6::Ipv6GetHopLimit(ipv6_hdr *header){
    return header->hopLimit;
}

quint8 DissectorIpv6::Ipv6GetHopLimit(DissRes *dissRes){
    return Ipv6GetHopLimit(Ipv6GetHdr(dissRes));
}

//Msg方法
QString DissectorIpv6::Ipv6MsgSummery(ipv6_hdr *header,DissResEth *dissResEth){
    return QString::asprintf("Internet Protocol Version : %d,",Ipv6GetVersion(header))
            .append(" Src: ").append(dissResEth->GetStrSrc())
            .append(" , Dst: ").append(dissResEth->GetStrDst());
}

QString DissectorIpv6::Ipv6MsgVersion(ipv6_hdr *header){
    Bit bit;
    bit.Set(Ipv6GetVersion(header));
    return QString::asprintf("%d%d%d%d .... = Version : %d"
                             ,bit.GetBit(3)
                             ,bit.GetBit(2)
                             ,bit.GetBit(1)
                             ,bit.GetBit(0)
                             ,Ipv6GetVersion(header)
                             );
}

void DissectorIpv6::Ipv6DealTrafficClass(ipv6_hdr *header, ProTree *tree, qint32 *start){
    QString msg;
    Bit bit(Ipv6GetTrafficClass(header));
    msg.append(QString::asprintf(".... %d%d%d%d %d%d%d%d .... .... .... .... .... = Traffic Class"
                        ,bit.GetBit(7)
                        ,bit.GetBit(6)
                        ,bit.GetBit(5)
                        ,bit.GetBit(4)
                        ,bit.GetBit(3)
                        ,bit.GetBit(2)
                        ,bit.GetBit(1)
                        ,bit.GetBit(0)
                        ));
    tree->AddItem("ipv6",msg,start,IPV6_LEN::VTS,false);

    msg.clear();
    msg.append(QString::asprintf(".... %d%d%d%d %d%d.. .... .... .... .... .... = Differentiated Services Codepoint"
                        ,bit.GetBit(5)
                        ,bit.GetBit(4)
                        ,bit.GetBit(3)
                        ,bit.GetBit(2)
                        ,bit.GetBit(1)
                        ,bit.GetBit(0)
                        ));
    tree->AddItem("ipv6",msg,start,IPV6_LEN::VTS,false,ProTree::level::NEW);

    msg.clear();
    msg.append(QString::asprintf(".... .... ..%d%d .... .... .... .... .... = Explicit Congestion Notification"
                        ,bit.GetBit(1)
                        ,bit.GetBit(0)
                        ));
    tree->AddItem("ipv6",msg,start,IPV6_LEN::VTS,false);
    tree->Pop();
}

QString DissectorIpv6::Ipv6MsgFlowLabel(ipv6_hdr *header){
    quint32 flowLabel = Ipv6GetFlowLabel(header);
    Bit bit(flowLabel);
    quint8 *ptr = (quint8*)&flowLabel;
    QString hex = QString::asprintf("0x%02x%02x%02x"
                                    ,ptr[2]
                                    ,ptr[1]
                                    ,ptr[0]
                                    ).remove(2,1);
    return QString::asprintf(".... .... .... %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d = Flow Lable : "
                             ,bit.GetBit(19),bit.GetBit(18),bit.GetBit(17),bit.GetBit(16)
                             ,bit.GetBit(15),bit.GetBit(14),bit.GetBit(13),bit.GetBit(12)
                             ,bit.GetBit(11),bit.GetBit(10),bit.GetBit(9),bit.GetBit(8)
                             ,bit.GetBit(7),bit.GetBit(6),bit.GetBit(5),bit.GetBit(4)
                             ,bit.GetBit(3),bit.GetBit(2),bit.GetBit(1),bit.GetBit(0)
                             )
                             .append(hex);
}


QString DissectorIpv6::Ipv6MsgPayloadLength(ipv6_hdr *header){
    return QString::asprintf("Payload length : %d",Ipv6GetPayloadLength(header));
}

QString DissectorIpv6::Ipv6MsgNextHeader(ipv6_hdr *header){
    QString type;
    quint8 typeNo = Ipv6GetNextHeader(header);
    switch (typeNo) {
        case 58:
            type.append("ICMPv6");
            break;
        default:
            type.append("Ipv6 Dissector: unknow next header");
    }
    return QString::asprintf("Next Header : ").append(type).append(QString::asprintf("(%d)",typeNo));
}

QString DissectorIpv6::Ipv6MsgHopLimit(ipv6_hdr *header){
    return QString::asprintf("Hop Limit : %d",Ipv6GetHopLimit(header));
}

QString DissectorIpv6::Ipv6MsgSourceAddress(DissResEth *dissResEth){
    return QString("Source: ").append(dissResEth->GetStrSrc());
}

QString DissectorIpv6::Ipv6MsgDestinationAddress(DissResEth *dissResEth){
    return QString("Destination: ").append(dissResEth->GetStrDst());
}

















