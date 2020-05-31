#include "dissectresultarp.h"

using namespace tcp_ip_protocol_family;

DissectResultArp::DissectResultArp(DissectResultBase *dissectResultBase,void *reserves)
        :DissectResult(dissectResultBase)
{
    Q_UNUSED(reserves)
    dissectResultBase->PushToProtocolList("arp",ARP_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("arp");

    this->GetDissectResultBase()->SetSummery(this->GetSummery());
}

quint16 DissectResultArp::GetHardwareType(){
    return ntohs(*(quint16*)this->header->hardwareType);
}

QString DissectResultArp::GetHardwareTypeStr(){
    return pcap_datalink_val_to_name(this->GetHardwareType());
}

QString DissectResultArp::GetProtocolTypeName(){
    if( *((quint16*)this->header->protocolType) == 0x0008 )
        return "ipv4";
    else
        return "nuknow";
}

QString DissectResultArp::GetProtocolTypeStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->protocolType,ARP_FIELD_LENGTH::PROTOCOLTYPE);
}

quint8 DissectResultArp::GetMacAddrLen(){
    return *this->header->macAddrLen;
}

quint8 DissectResultArp::GetProtocolAddrLen(){
    return *this->header->protocolAddrLen;
}

quint16 DissectResultArp::GetOpCode(){
    return ntohs(*((quint16*)this->header->opcode));
}

QString DissectResultArp::GetOpereation(){
    if( this->GetOpCode() == 1 )
        return "request";
    else if( this->GetOpCode() == 2 )
        return "reply";
    else
        return "unknow";
}

QString DissectResultArp::GetSrcMacStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->srcMac,ARP_FIELD_LENGTH::SRCMAC,":","");
}

QString DissectResultArp::GetSrcIpStr(){
    return Converter::ConvertQuint8ArrayToDecStr(this->header->srcIp,ARP_FIELD_LENGTH::SRCIP,".","");
}

QString DissectResultArp::GetDstMacStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->dstMac,ARP_FIELD_LENGTH::DSTMAC,":","");
}

QString DissectResultArp::GetDstIpStr(){
    return Converter::ConvertQuint8ArrayToDecStr(this->header->dstIp,ARP_FIELD_LENGTH::DSTIP,".","");
}

QString DissectResultArp::GetSummery(){
    if( this->GetOpCode() == 1 )
        return QString("Who has %1 ? Tell %2")
                .arg(this->GetDstIpStr())
                .arg(this->GetSrcIpStr());
    else if( this->GetOpCode() == 2 )
        return QString("%1 is at %2")
                .arg(this->GetSrcIpStr())
                .arg(this->GetSrcMacStr());
    else
        return "unknow opcode";
}
