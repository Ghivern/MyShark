#include "dissectresultipv4.h"

using namespace tcp_ip_protocol_family;

DissectResultIpv4::DissectResultIpv4(DissectResultBase *dissectResultBase){
    this->protocol_family_transport_layer = NULL;
    this->dissectResultBase = dissectResultBase;
    dissectResultBase->PushToProtocolList("ipv4",NETWORKLAYER_IPV4_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ipv4");
    if(this->header != NULL){
        dissectResultBase->UpdateProtocolList("ipv4",this->GetHeaderLength());
        dissectResultBase->UpdateProtocolHeaderLengthCount(this->GetHeaderLength());
        this->AddNextLayer(dissectResultBase,(NETWORKLAYER_IPV4_PROTOCOL_TYPE)*header->type);
    }
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




/*
 *  以下均是解析Ipv4首部字段的方法,调用前提是，this->header已经被正确赋值
 *
 *0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *Version|Hdr Len|   DS          |     total length              |
 *   identification              |flag | offset                  |
 *   ttl         | protocol      |    header checksum            |
 *                src address                                    |
 *                dst address                                    |
 * options            .  padding                                 |
 *                    data                                       |
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * 他
 * Hdr Len :单位是4字节
 * DS: Differntiated Service   RFC 2474 3168 3260
 * offset:  片偏移
 * ttl : time to live
 * 考虑到Ipv4的首部拓展很少使用，所以暂时不做处理
 */
quint8 DissectResultIpv4::GetHeaderLength(){
    return this->header->version_hdrLen & 0x0f;
}
