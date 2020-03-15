#include "dissectorarp.h"

quint32 DissectorArp::flags = 0;

DissectorArp::DissectorArp()
{

}

void DissectorArp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    arp_hdr *header = GetArpHdr(dissRes);
    if(info == NULL){
        qDebug() << "DissectorArp : info == NULL";
        DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissResEth->AddToProtocolStackWithSE("arp",sizeof(arp_hdr));
        dissResEth->SetMsg(MsgArpInfo(header));
    }else{
        Q_UNUSED(proTree)
    }
}

//Get 方法
arp_hdr* DissectorArp::DissectorArp::GetArpHdr(DissRes *dissRes){
    arp_hdr *header = (arp_hdr*)(dissRes->GetData() + dissRes->GetHeadersLen());
    dissRes->AddHeadersLen(sizeof(arp_hdr));
    return header;
}

QString DissectorArp::GetStrAprHType(arp_hdr *header){
    return pcap_datalink_val_to_name(ntohs(header->hardware_type));
}
ushort DissectorArp::GetArpHType(arp_hdr *header){
    return ntohs(header->hardware_type);
}

QString DissectorArp::GetStrArpPType(arp_hdr *header){
    switch (GetArpPType(header)) {
        case (ushort)0x0800:
            return "IPv4";
        default:
            return "ARP:(Unknow)" + QString::asprintf("%02x%02x"
                                                             ,(&header->protocol_type)[0]
                                                             ,(&header->protocol_type)[1]
                            );
    }
}
ushort DissectorArp::GetArpPType(arp_hdr *header){
    return ntohs(header->protocol_type);
}
uchar DissectorArp::GetArpHLength(arp_hdr *header){
    return header->hardware_address_len;
}
uchar DissectorArp::GetArpPLength(arp_hdr *header){
    return header->protocol_address_len;
}
ushort DissectorArp::GetArpOpType(arp_hdr *header){
    return ntohs(header->op);
}
QString DissectorArp::GetStrArpOpType(arp_hdr *header){
    switch (GetArpOpType(header)) {
        case (ushort)1:
            return "Request";
        case (ushort)2:
            return "Reply";
        default:
            return "未知ARP操作类型，值为" + QString(GetArpOpType(header));
    }
}
QString DissectorArp::GetStrArpHSrcAddress(arp_hdr *header){
    return QString::asprintf("%02x%02x%02x%02x%02x%02x"
            ,header->src_hardware[0]
            ,header->src_hardware[1]
            ,header->src_hardware[2]
            ,header->src_hardware[3]
            ,header->src_hardware[4]
            ,header->src_hardware[5]
            );
}
QString DissectorArp::GetStrArpPSrcAddress(arp_hdr *header){
    return QString::asprintf("%d.%d.%d.%d"
            ,header->src_protocol[0]
            ,header->src_protocol[1]
            ,header->src_protocol[2]
            ,header->src_protocol[3]
            );
}
QString DissectorArp::GetStrArpHDstAddress(arp_hdr *header){
    return QString::asprintf("%02x%02x%02x%02x%02x%02x"
            ,header->dst_hardware[0]
            ,header->dst_hardware[1]
            ,header->dst_hardware[2]
            ,header->dst_hardware[3]
            ,header->dst_hardware[4]
            ,header->dst_hardware[5]
            );
}
QString DissectorArp::GetStrArpPDstAddress(arp_hdr *header){
    return QString::asprintf("%d.%d.%d.%d"
            ,header->dst_protocol[0]
            ,header->dst_protocol[1]
            ,header->dst_protocol[2]
            ,header->dst_protocol[3]
            );
}

//Msg方法
QString DissectorArp::MsgArpInfo(arp_hdr *header){
    switch (GetArpOpType(header)) {
        case (ushort)1:
            return "Who has " + GetStrArpPDstAddress(header) + "?    "
                    + "Tell " + GetStrArpPSrcAddress(header);
        case (ushort)2:
            return GetStrArpPSrcAddress(header) + " is at "
                    + GetStrArpHSrcAddress(header);
        default:
            return "未知ARP操作类型，值为" + QString(GetArpOpType(header));
    }
}
