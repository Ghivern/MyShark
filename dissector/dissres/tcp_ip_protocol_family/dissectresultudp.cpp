#include "dissectresultudp.h"

using namespace tcp_ip_protocol_family;

Stream DissectResultUdp::stream;

DissectResultUdp::DissectResultUdp(DissectResultBase *dissectResultBase,void *reserves):DissectResult(dissectResultBase)
{
    quint64 options = DissectResultBase::GetDissectorOptionPtr()->value("udp");
    Q_UNUSED(options)

    dissectResultBase->PushToProtocolList("udp",TRANSPORTLAYER_UDP_FIELD_LENGTH_TEMP_HEADER_LENGTH);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("udp");

    if( this->GetDissectResultBase()->ContainProtocol("ipv4") ){
        Ipv4Info *ipv4Info = (Ipv4Info*)this->GetDissectResultBase()->GetAdditionalPtr(IPV4_INFO_PTR);
        this->SetStremIndexPlusOne(
                    stream.Add(dissectResultBase
                               ,ipv4Info->src
                               ,ipv4Info->dst
                               ,ipv4Info->addrLen
                               ,this->GetSourcePortPtr()
                               ,this->GetDestinationPortPtr()
                               ,TRANSPORTLAYER_UDP_FIELD_LENGTH_SOURCE_PORT)
                    );
    }else{
        this->SetStremIndexPlusOne(0);
    }

    if( options & TCP_VALIDATE_CHECKSUM ){
        //计算校验和
    }

    if( header != nullptr )
        this->addNextLayer(dissectResultBase,reserves);
}

quint8* DissectResultUdp::GetSourcePortPtr(){
    return this->header->srcPort;
}

quint8* DissectResultUdp::GetDestinationPortPtr(){
    return this->header->dstPort;
}

quint16 DissectResultUdp::GetSourcePort(){
    return ntohs(*(quint16*)this->header->srcPort);
}

quint16 DissectResultUdp::GetDestinationPort(){
    return ntohs(*(quint16*)this->header->dstPort);
}

Stream& DissectResultUdp::GetStreamRecorder(){
    return DissectResultUdp::stream;
}

void DissectResultUdp::addNextLayer(DissectResultBase *dissectResultBase, void *reserves){
    Q_UNUSED(dissectResultBase)
    Q_UNUSED(reserves)
}
