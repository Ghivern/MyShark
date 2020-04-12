#include "dissectresutltframe.h"

bool DissectResultFrame::isFirstPacket = true;
timeval DissectResultFrame::firstPacketCaptureTime;

DissectResultFrame::DissectResultFrame(const quint8 *data, const pcap_pkthdr *pkthdr, qint64 index, PROTOCOL_FAMILY_TYPE protocol_family_type){
    //this->packet.data = (quint8*)malloc(pkthdr->caplen);
    quint8 *dst_data = (quint8*)malloc(pkthdr->caplen);
    memcpy(dst_data,data,pkthdr->caplen);
    this->dissectResultBase->SetData(dst_data);
    this->pkthdr = (pcap_pkthdr*)malloc(sizeof(pcap_pkthdr));
    memcpy(this->pkthdr,pkthdr,sizeof(pcap_pkthdr));

    this->dissectResultBase = new DissectResultBase(this->packet.data,index);

    this->protocol_family_type = protocol_family_type;
    switch (protocol_family_type)
    {
    case PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY:
        this->protocol_family_base_layer = (void*)(new tcp_ip_protocol_family::DissectResultLinkLayer(this->dissectResultBase));
        break;
    default:
        this->protocol_family_base_layer = NULL;
        break;
    }

    if(isFirstPacket){
        firstPacketCaptureTime.tv_sec = pkthdr->ts.tv_sec;
        firstPacketCaptureTime.tv_usec = pkthdr->ts.tv_usec;
        isFirstPacket = false;
    }
}

void DissectResultFrame::SetSummery(QString summery){
    this->dissectResultBase->SetSummery(summery);
}

void DissectResultFrame::UpdateProtocolHeaderLengthCount(qint32 headerLength){
    //this->protocolHeaderLengthCount += headerLength;
    this->dissectResultBase->UpdateProtocolHeaderLengthCount(headerLength);
}

void DissectResultFrame::PushToProtocolList(QString protocolName, qint32 protocolHeaderLength){
    this->dissectResultBase->PushToProtocolList(protocolName,protocolHeaderLength);
}

void DissectResultFrame::UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength){
    this->dissectResultBase->UpdateProtocolList(protocolName,newProtocolHeaderLength);
}

const quint8* DissectResultFrame::GetData(){
    return this->dissectResultBase->GetData();
}

qint64 DissectResultFrame::GetIndex(){
    //return this->index;
    return this->dissectResultBase->GetIndex();
}

quint64 DissectResultFrame::GetCaptureTimeSec(){
    return this->pkthdr->ts.tv_sec;
}

quint64 DissectResultFrame::GetCaptureTimeUSec(){
    return this->pkthdr->ts.tv_usec;
}

double DissectResultFrame::GetRelativeTimeSinceFirstPacket(){
    return (this->GetCaptureTimeSec() - DissectResultFrame::firstPacketCaptureTime.tv_sec)
            +
           (this->GetCaptureTimeUSec() - DissectResultFrame::firstPacketCaptureTime.tv_usec)/1000000.0;
}

qint32 DissectResultFrame::GetCapLen(){
    return this->pkthdr->caplen;
}

qint32 DissectResultFrame::GetLen(){
    return this->pkthdr->len;
}

qint32 DissectResultFrame::GetProtocolHeaderLengthCount(){
    return this->dissectResultBase->GetProtocolHeaderLengthCount();
}

void* DissectResultFrame::GetProtocolFamilyBaseLayer(){
    return this->protocol_family_base_layer;
}

DissectResultFrame* DissectResultFrame::GetTcpIpProtocolFamilyBaseLayer(){
    return (DissectResultFrame*)this->protocol_family_base_layer;
}

QString DissectResultFrame::GetSummery(){
    return this->dissectResultBase->GetSummery();
}

qint32 DissectResultFrame::GetProtocolListLength(){
    return this->dissectResultBase->GetProtocolListLength();
}

QString DissectResultFrame::GetProtocolNameByIndex(qint32 index){
    return this->dissectResultBase->GetProtocolNameByIndex(index);
}

const quint8* DissectResultFrame::GetProtocolHeaderStartPtrByName(QString protocolName){
    return this->dissectResultBase->GetProtocolHeaderStartPtrByName(protocolName);
}
