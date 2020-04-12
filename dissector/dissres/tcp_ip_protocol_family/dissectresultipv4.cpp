#include "dissectresultipv4.h"

using namespace tcp_ip_protocol_family;

DissectResultIpv4::DissectResultIpv4(DissectResultBase *dissectResultBase){
    this->protocol_family_transport_layer = NULL;
    this->dissectResultBase = dissectResultBase;
    dissectResultBase->PushToProtocolList("ipv4",NETWORKLAYER_IPV4_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ipv4");
    this->AddNextLayer(dissectResultBase,(NETWORKLAYER_IPV4_PROTOCOL_TYPE)*header->type);
}

void DissectResultIpv4::AddNextLayer(DissectResultBase *dissectResultBase, NETWORKLAYER_IPV4_PROTOCOL_TYPE type){
    switch (type) {
        case NETWORKLAYER_IPV4_TCP:
        this->protocol_family_transport_layer = (void*)(new DissectResultTcp(dissectResultBase));
        break;
    case NETWORKLAYER_IPV4_UDP:
        this->protocol_family_transport_layer = (void*)(new DissectResultUdp(dissectResultBase));
        break;
    }
}

void* DissectResultIpv4::GetNextLayer(){
    return this->protocol_family_transport_layer;
}

DissectResultTcp* DissectResultIpv4::GetNextLayerTcp(){
    return (DissectResultTcp*)this->protocol_family_transport_layer;
}

DissectResultUdp* DissectResultIpv4::GetNextLayerUdp(){
    return (DissectResultUdp*)this->protocol_family_transport_layer;
}
