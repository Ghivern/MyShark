#include "dissectresulttcp.h"
#include <QtDebug>

using namespace tcp_ip_protocol_family;

StreamTcp DissectResultTcp::stream;

DissectResultTcp::DissectResultTcp(DissectResultBase *dissectResultBase)
{
    this->dissectResultBase = dissectResultBase;
    this->total_length = dissectResultBase->GetAdditionalVal(TCP_TOTAL_LEN);

    dissectResultBase->PushToProtocolList("tcp",TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("tcp");

    dissectResultBase->UpdateProtocolList("tcp",this->GetOffset() * 4);

    dissectResultBase->AddAdditional(TCP_ISSYN,this->SYN()?1:0);
    dissectResultBase->AddAdditional(TCP_SEQ_VAL,this->GetSeq());
    dissectResultBase->AddAdditional(TCP_ACK_VAL,this->GetAck());
    dissectResultBase->AddAdditional(TCP_PAYLOAD_LEN,this->GetPayloadLen());
    this->streamIndexPlusOne = stream.AddWithWindow(dissectResultBase
                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_SOURCE_ADDRESS_PTR)
                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_DESTINATION_ADDRESS_PTR)
                         ,dissectResultBase->GetAdditionalVal(IP_ADDRESS_LENGTH)
                         ,this->GetSourcePortPtr()
                         ,this->GetDestinationPortPtr()
                         ,TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT
                         );
    dissectResultBase->SetSummery(QString("%1 -> %2").arg(this->GetSourcePort()).arg(this->GetDestinationPort()));
    //qDebug() << "streamIndexPlusOne = " << this->streamIndexPlusOne;
}


/*处理端口号*/
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

/*处理Seq和Ack*/
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

/*处理首部长度和负载长度*/
quint8 DissectResultTcp::GetOffset(){
    return (this->header->offset_reserved_flags[0] & 0xf0) >> 4;
}

quint32 DissectResultTcp::GetPayloadLen(){
    return this->total_length - this->GetOffset() * 4;
}

/*Flags*/
bool DissectResultTcp::SYN(){
    if(((this->header->offset_reserved_flags[1] & 0x02) >> 1) == 1)
        return true;
    else
        return false;
}

/*分析Seq/Ack*/
QString DissectResultTcp::GetSegmentStatusStr(){
    qint32 status = this->dissectResultBase->GetAdditionalVal(TCP_STATUS);
    if(status == -1)
        return "normal";
    switch (status) {
    case StreamTcp::TCP_A_OUT_OF_ORDER:
        return "out of order";
    case StreamTcp::TCP_A_RETRANSMISSION:
        return "retransmission";
    case StreamTcp::TCP_A_FAST_RETRANSMISSION:
        return "fast retransmission";
    default:
        return "normal";
    }
}

/*从DissectResultBase的保留字段获取数据，
*取得分片的前一分片的包Index，若无前一个分片，或此分片长度为0，
* 返回-1
*/
qint64 DissectResultTcp::GetPrevious(){
    return this->dissectResultBase->GetAdditionalVal(TCP_PRE_SEGMENT);
}
