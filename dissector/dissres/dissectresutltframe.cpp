#include "dissectresutltframe.h"




bool DissectResultFrame::isFirstPacket = true;
timeval DissectResultFrame::firstPacketCaptureTime;
QList<DissectResultBase*>* DissectResultFrame::dissectResultBaseList = new QList<DissectResultBase*>;

DissectResultFrame::DissectResultFrame(const quint8 *data
                                       ,const pcap_pkthdr *pkthdr
                                       ,qint64 index
                                       ,PROTOCOL_FAMILY_TYPE protocol_family_type
                                       ,void *reserves)
{
    if( index == 0 && this->dissectResultBaseList->length() > 0){
        this->dissectResultBaseList->clear();
        this->isFirstPacket = true;
    }

    Q_UNUSED(reserves)
    quint8 *dst_data = (quint8*)malloc(pkthdr->caplen);
    memcpy(dst_data,data,pkthdr->caplen);
    pcap_pkthdr *dst_pkthdr = (pcap_pkthdr*)malloc(sizeof(pcap_pkthdr));
    memcpy(dst_pkthdr,pkthdr,sizeof(pcap_pkthdr));
    this->pkthdr = dst_pkthdr;

    this->dissectResultBase = new DissectResultBase(dst_data,dst_pkthdr,index);
    DissectResultFrame::dissectResultBaseList->append(this->dissectResultBase);
    this->dissectResultBase->AddAdditional(DISSECT_RESULT_BASE_LIST,dissectResultBaseList);

//    this->protocol_family_type = protocol_family_type;
//    switch (protocol_family_type)
//    {
//    case PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY:
//        this->protocol_family_base_layer =
//                (void*)(new tcp_ip_protocol_family::DissectResultLinkLayer(this->dissectResultBase
//                                                                           ,reserves));
//        break;
//    default:
//        this->protocol_family_base_layer = nullptr;
//    }

    if(isFirstPacket){
        firstPacketCaptureTime.tv_sec = pkthdr->ts.tv_sec;
        firstPacketCaptureTime.tv_usec = pkthdr->ts.tv_usec;
        isFirstPacket = false;
    }

    this->addFirstLayer(protocol_family_type,reserves);
}

//void DissectResultFrame::SetSummery(QString summery){
//    this->dissectResultBase->SetSummery(summery);
//}

//void DissectResultFrame::UpdateProtocolHeaderLengthCount(qint32 headerLength){
//    this->dissectResultBase->UpdateProtocolHeaderLengthCount(headerLength);
//}

//void DissectResultFrame::PushToProtocolList(QString protocolName, qint32 protocolHeaderLength){
//    this->dissectResultBase->PushToProtocolList(protocolName,protocolHeaderLength);
//}

//void DissectResultFrame::UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength){
//    this->dissectResultBase->UpdateProtocolList(protocolName,newProtocolHeaderLength);
//}

//const quint8* DissectResultFrame::GetData(){
//    return this->dissectResultBase->GetData();
//}

//quint64 DissectResultFrame::GetIndex(){
//    return this->dissectResultBase->GetIndex();
//}

qint64 DissectResultFrame::GetCaptureTimeSec(){
    return this->pkthdr->ts.tv_sec;
}

qint64 DissectResultFrame::GetCaptureTimeUSec(){
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

//qint32 DissectResultFrame::GetProtocolHeaderLengthCount(){
//    return this->dissectResultBase->GetProtocolHeaderLengthCount();
//}

DissectResultBase* DissectResultFrame::GetDissectResultBase(){
    return this->dissectResultBase;
}

void* DissectResultFrame::GetProtocolFamilyBaseLayer(){
    return this->protocol_family_base_layer;
}

tcp_ip_protocol_family::DissectResultLinkLayer* DissectResultFrame::GetTcpIpProtocolFamilyBaseLayer(){
    return (tcp_ip_protocol_family::DissectResultLinkLayer*)this->protocol_family_base_layer;
}

void DissectResultFrame::Clear(){
    tcp_ip_protocol_family::DissectResultLinkLayer::Clear();
}

//QString DissectResultFrame::GetSummery(){
//    return this->dissectResultBase->GetSummery();
//}

//qint32 DissectResultFrame::GetProtocolListLength(){
//    return this->dissectResultBase->GetProtocolListLength();
//}

//QString DissectResultFrame::GetProtocolNameByIndex(qint32 index){
//    return this->dissectResultBase->GetProtocolNameByIndex(index);
//}

//const quint8* DissectResultFrame::GetProtocolHeaderStartPtrByName(QString protocolName){
//    return this->dissectResultBase->GetProtocolHeaderStartPtrByName(protocolName);
//}

//bool DissectResultFrame::ContainProtocol(QString proName){
//    return this->dissectResultBase->ContainProtocol(proName);
//}

//QString DissectResultFrame::GetTopProtocolName(){
//    return this->dissectResultBase->GetTopProtocolName();
//}

void DissectResultFrame::addFirstLayer(PROTOCOL_FAMILY_TYPE type, void *reserves){
    switch (type)
    {
    case PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY:
        this->protocol_family_base_layer =
                (void*)(new tcp_ip_protocol_family::DissectResultLinkLayer(this->dissectResultBase
                                                                           ,reserves));
        break;
    default:
        this->protocol_family_base_layer = nullptr;
    }
}
