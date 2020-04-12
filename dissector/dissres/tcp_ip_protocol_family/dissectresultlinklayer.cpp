#include "dissectresultlinklayer.h"

using namespace tcp_ip_protocol_family;

DissectResultLinkLayer::DissectResultLinkLayer(DissectResultBase *dissectResultBase){
    this->protocol_family_network_layer = NULL;
    this->dissectResultBase = dissectResultBase;
    dissectResultBase->PushToProtocolList("ether",LINKLAYER_FIELD_LENGTH::LINKLAYER_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ether");
    dissectResultBase->UpdateProtocolHeaderLengthCount(LINKLAYER_TEMP_TOTAL_LEN);
    this->AddNextLayer(dissectResultBase,(LINKLAYER_PROTOCOL_TYPE)*(quint16*)header->type);
}

void DissectResultLinkLayer::AddNextLayer(DissectResultBase *dissectResultBase, LINKLAYER_PROTOCOL_TYPE type){
    switch(type){
    case LINKLAYER_IPV4:
        this->protocol_family_network_layer = (void*)(new DissectResultIpv4(dissectResultBase));
        break;
    case LINKLAYER_IPV6:
        this->protocol_family_network_layer = (void*)(new DissectResultIpv6(dissectResultBase));
        break;
    case LINKLAYER_ARP:
        this->protocol_family_network_layer = (void*)(new DissectResultArp(dissectResultBase));
        break;
    }
}

void* DissectResultLinkLayer::GetNextLayer(){
    return this->protocol_family_network_layer;
}

DissectResultIpv4* DissectResultLinkLayer::GetNextLayerIpv4(){
    return (DissectResultIpv4*)this->GetNextLayer();
}

DissectResultIpv6* DissectResultLinkLayer::GetNextLayerIpv6(){
    return (DissectResultIpv6*)this->GetNextLayer();
}

DissectResultArp* DissectResultLinkLayer::GetNextLayerArp(){
    return (DissectResultArp*)this->GetNextLayer();
}
