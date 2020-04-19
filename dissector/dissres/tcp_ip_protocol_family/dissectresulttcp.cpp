#include "dissectresulttcp.h"
#include <QtDebug>

using namespace tcp_ip_protocol_family;

StreamTcp DissectResultTcp::stream;

DissectResultTcp::DissectResultTcp(DissectResultBase *dissectResultBase)
{
    this->dissectResultBase = dissectResultBase;
    this->total_length = dissectResultBase->GetAdditionalVal("tcp_total_len");

    dissectResultBase->PushToProtocolList("tcp",TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("tcp");

    dissectResultBase->UpdateProtocolList("tcp",this->GetOffset() * 4);

    dissectResultBase->AddAdditional("issyn",this->SYN()?1:0);
    dissectResultBase->AddAdditional("seq",this->GetSeq());
    dissectResultBase->AddAdditional("ack",this->GetAck());
    dissectResultBase->AddAdditional("len",this->GetPayloadLen());
    this->streamIndexPlusOne = stream.AddWithWindow(dissectResultBase
                         ,(quint8*)dissectResultBase->GetAdditionalPtr("srcAddr")
                         ,(quint8*)dissectResultBase->GetAdditionalPtr("dstAddr")
                         ,dissectResultBase->GetAdditionalVal("addrLen")
                         ,this->GetSourcePortPtr()
                         ,this->GetDestinationPortPtr()
                         ,TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT
                         );
    //qDebug() << "streamIndexPlusOne = " << this->streamIndexPlusOne;
}

quint8* DissectResultTcp::GetSourcePortPtr(){
    return this->header->srcPort;
}

quint16 DissectResultTcp::GetSourcePort(){
    return ntohs(*(quint16*)header->srcPort);
}

quint8* DissectResultTcp::GetDestinationPortPtr(){
    return this->header->dstPort;
}

quint16 DissectResultTcp::GetDestinationPort(){
    return ntohs(*(quint16*)header->dstPort);
}

quint32 DissectResultTcp::GetSeq(){
    return ntohl(*(quint32*)this->header->seq);
}

quint32 DissectResultTcp::GetRelativeSeq(){
    return this->GetSeq() - DissectResultTcp::stream.GetBaseSeq(this->streamIndexPlusOne);
}

quint32 DissectResultTcp::GetAck(){
    return ntohl(*(quint32*)this->header->ack);
}

quint32 DissectResultTcp::GetRelativeAck(){
    return this->GetAck() - DissectResultTcp::stream.GetBaseSeq(-this->streamIndexPlusOne);
}

quint8 DissectResultTcp::GetOffset(){
    return (this->header->offset_reserved_flags[0] & 0xf0) >> 4;
}

quint32 DissectResultTcp::GetPayloadLen(){
    return this->total_length - this->GetOffset() * 4;
}

bool DissectResultTcp::SYN(){
    if(((this->header->offset_reserved_flags[1] & 0x02) >> 1) == 1)
        return true;
    else
        return false;
}

QString DissectResultTcp::GetSegmentStatusStr(){
    qint32 status = this->dissectResultBase->GetAdditionalVal(SEGMENT_STATUS);
    if(status == -1)
        return "normal";
    switch (status) {
    case StreamTcp::TCP_A_OUT_OF_ORDER:
        return "out of order";
    case StreamTcp::TCP_A_RETRANSMISSION:
        return "retransmission";
    default:
        return "normal";
    }
}

qint64 DissectResultTcp::GetPrevious(){
    return this->dissectResultBase->GetAdditionalVal(PRE_SEGMENT);
}
