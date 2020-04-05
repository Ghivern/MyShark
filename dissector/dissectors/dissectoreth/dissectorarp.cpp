#include "dissectorarp.h"

quint32 DissectorArp::flags = 0;

DissectorArp::DissectorArp()  {}

void DissectorArp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
        arp_hdr *header = DissectorArp::GetArpHdr(dissRes,info==NULL?true:false);
    if(info == NULL){
        qDebug() << "DissectorArp : info == NULL";
        //DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissRes->AddHeadersLen(sizeof(arp_hdr));
        dissRes->SetMsg(MsgArpInfo(header));
    }else{
        qint32 start = dissRes->GetProStart("arp");
        proTree->AddItem("arp",DissectorArp::MsgTopLevel(header),dissRes->GetProStart("arp"),dissRes->GetProEnd("arp"));

        proTree->AddItem("arp",DissectorArp::MsgHdType(header),start,start+1,ProTree::NEW);
        start += 2;

        proTree->AddItem("arp",DissectorArp::MsgProType(header),start,start+1);
        start +=2;

        proTree->AddItem("arp",DissectorArp::MsgHdSize(header),start,start+0);
        start +=1;

        proTree->AddItem("arp",DissectorArp::MsgProSize(header),start,start+0);
        start +=1;

        proTree->AddItem("arp",DissectorArp::MsgOpcode(header),start,start+1);
        start +=2;

        proTree->AddItem("arp",DissectorArp::MsgSenderMac(header),start,start+5);
        start +=6;

        proTree->AddItem("arp",DissectorArp::MsgSenderIp(header),start,start+3);
        start +=4;

        proTree->AddItem("arp",DissectorArp::MsgTargetMac(header),start,start+5);
        start +=6;

        proTree->AddItem("arp",DissectorArp::MsgTargetIp(header),start,start+3);
        start +=4;

        proTree->Pop();

    }
}

//Get 方法
arp_hdr* DissectorArp::DissectorArp::GetArpHdr(DissRes *dissRes,bool first){
    if(first)
        dissRes->AddToProtocolStackWithSE("arp",sizeof(arp_hdr));
    arp_hdr *header = (arp_hdr*)(dissRes->GetData() + dissRes->GetProStart("arp"));
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
    return QString::asprintf("%02x.%02x.%02x.%02x.%02x.%02x"
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
    return QString::asprintf("%02x.%02x.%02x.%02x.%02x.%02x"
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

QString DissectorArp::MsgTopLevel(arp_hdr *header){
    ushort op = ntohs(header->op);
    return QString::asprintf("Address Resolution Protocol (") + ((op == 1)?"Request)":"Reply)") ;
}

QString DissectorArp::MsgHdType(arp_hdr *header){
    return "Hardware type :"  + DissectorArp::GetStrAprHType(header);
}

QString DissectorArp::MsgProType(arp_hdr *header){
    ushort type = ntohs(header->protocol_type);
    QString strtype = DissectorArp::GetStrArpPType(header);
    return QString("Protocol type : ") + strtype + QString::asprintf("(0x%02x%02x)"
                                                                     ,((uchar*)&type)[1]
                                                                     ,((uchar*)&type)[0]);
}

QString DissectorArp::MsgHdSize(arp_hdr *header){
    return QString::asprintf("Hardware size : %d",header->hardware_address_len);
}

QString DissectorArp::MsgProSize(arp_hdr *header){
    return QString::asprintf("Protocol size : %d",header->protocol_address_len);
}

QString DissectorArp::MsgOpcode(arp_hdr *header){
    ushort op = ntohs(header->op);
    return QString::asprintf("Opcode : ") + ((op == 1)?"Request":"Reply")
            + QString::asprintf(" (%d)",op);
}

QString DissectorArp::MsgSenderMac(arp_hdr *header){
    return QString("Sender Mac address : ") + DissectorArp::GetStrArpHSrcAddress(header);
}

QString DissectorArp::MsgSenderIp(arp_hdr *header){
    return QString("Sender Ip address : ") + DissectorArp::GetStrArpPSrcAddress(header);
}

QString DissectorArp::MsgTargetMac(arp_hdr *header){
    return QString("Target Mac address : ") + DissectorArp::GetStrArpHDstAddress(header);
}

QString DissectorArp::MsgTargetIp(arp_hdr *header){
    return QString("Target IP address : ") + DissectorArp::GetStrArpPDstAddress(header);
}
