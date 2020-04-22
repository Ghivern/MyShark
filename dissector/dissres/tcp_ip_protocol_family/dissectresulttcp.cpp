#include "dissectresulttcp.h"
#include <QtDebug>

using namespace tcp_ip_protocol_family;

StreamTcp DissectResultTcp::stream;

DissectResultTcp::DissectResultTcp(DissectResultBase *dissectResultBase)
{
    this->dissectResultBase = dissectResultBase;

    dissectResultBase->PushToProtocolList("tcp",TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("tcp");

    dissectResultBase->UpdateProtocolList("tcp",this->GetOffset() * 4);

    this->dealTcpOptions();

    dissectResultBase->AddAdditional(TCP_ISSYN,this->SYN()?1:0);
    dissectResultBase->AddAdditional(TCP_SEQ_VAL,this->GetSeq());
    dissectResultBase->AddAdditional(TCP_ACK_VAL,this->GetAck());
    dissectResultBase->AddAdditional(TCP_PAYLOAD_LEN,this->GetPayloadLen());
    dissectResultBase->AddAdditional(TCP_WINDOW_MULTIPLIER,this->GetOptionWindowMultiplier());
    this->streamIndexPlusOne = stream.AddWithWindow(dissectResultBase
                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_SOURCE_ADDRESS_PTR)
                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_DESTINATION_ADDRESS_PTR)
                         ,dissectResultBase->GetAdditionalVal(IP_ADDRESS_LENGTH)
                         ,this->GetSourcePortPtr()
                         ,this->GetDestinationPortPtr()
                         ,TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT
                         );
    dissectResultBase->SetSummery(QString("%1 -> %2").arg(this->GetSourcePort()).arg(this->GetDestinationPort()));
    dissectResultBase->SetSummery(QString("Window scale:%1 maxSeg:%2 tval:%3 ter:%4 %5 win:%6 c-win:%7")
                                  .arg(this->GetOptionWindowScale())
                                  .arg(this->GetOptionMaximumSegmentSize())
                                  .arg(this->GetOptionTimestampValue())
                                  .arg(this->GetOptionTimestampEchoReply())
                                  .arg(this->SYN()?"SYN":"")
                                  .arg(this->GetWindow())
                                  .arg(this->GetCalculatedWindow())
                                  );
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
    return this->dissectResultBase->GetAdditionalVal(TCP_TOTAL_LEN) - this->GetOffset() * 4;
}

/*Flags*/
bool DissectResultTcp::SYN(){
    if(((this->header->offset_reserved_flags[1] & 0x02) >> 1) == 1)
        return true;
    else
        return false;
}

/*Window*/
quint8* DissectResultTcp::GetWindowPtr(){
    return this->header->window;
}

quint16 DissectResultTcp::GetWindow(){
    return ntohs(*(quint16*)this->header->window);
}

quint32 DissectResultTcp::GetCalculatedWindow(){
    return this->GetWindow() * stream.GetWindowMultiplier(streamIndexPlusOne);
}

/*Checksum*/
QString DissectResultTcp::GetChecksumStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->checksum,TRANSPORTLAYER_TCP_FIELD_LENGTH_CHECKSUM);
}

/*Urgent Point*/
quint16 DissectResultTcp::GetUrgentPoint(){
    return ntohs(*(quint16*)this->header->urgentPoint);
}

/*
 * Tcp options
 */
qint32 DissectResultTcp::GetOptionPtrByIndex(quint8 *kind,quint8 *length, const quint8 **ptr, qint32 index){
    if(this->options_dsc.isEmpty() || !this->options_dsc.contains(index))
        return -1;
    *kind = this->options_dsc.value(index).kind;
    *length = this->options_dsc.value(index).length;
    *ptr = this->options_dsc.value(index).ptr;
    return 1;
}

qint32 DissectResultTcp::GetOptionMaximumSegmentSize(){
    qint32 index = this->getOptionIndex(TCP_OPTION_MAXIMUM_SEGMENT_SIZE);
    if(index != -1)
        return ntohs(*(quint16*)this->options_dsc.value(index).ptr);
    return -1;
}

qint16 DissectResultTcp::GetOptionWindowScale(){
    qint32 index = this->getOptionIndex(TCP_OPTION_WINDOW_SCALE_OPTION);
    if(index != -1)
        return *this->options_dsc.value(index).ptr;
    return -1;
}

qint16 DissectResultTcp::GetOptionWindowMultiplier(){
    qint16 scale = this->GetOptionWindowScale();
    if(scale != -1)
        return qPow(2,scale);
    return -1;
}

qint64 DissectResultTcp::GetOptionTimestampValue(){
    qint32 index = this->getOptionIndex(TCP_OPTION_TIMESTAMPS_OPTION);
    if(index != -1)
        return ntohl(*(quint32*)this->options_dsc.value(index).ptr);
    return -1;
}

qint64 DissectResultTcp::GetOptionTimestampEchoReply(){
    qint32 index = this->getOptionIndex(TCP_OPTION_TIMESTAMPS_OPTION);
    if(index != -1)
        return ntohl(*(quint32*)(this->options_dsc.value(index).ptr + 4));
    return -1;
}

/*
* 从DissectResultBase的保留字段获取数据，
*分析Seq/Ack
*/
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



/*Private*/

/*Tcp Options*/
void DissectResultTcp::dealTcpOptions(){
    const quint8 *startPtr = this->dissectResultBase->GetProtocolHeaderStartPtrByName("tcp") + TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH;
    qint32 index = 0;
    qint32 headLen = this->GetOffset() * 4;
    qint32 headLenCount = TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH;
    while (headLenCount < headLen) {
        struct option_dsc_t option_dsc;
        option_dsc.kind = *startPtr;

        if(option_dsc.kind == TCP_OPTION_NO_OPERATION || option_dsc.kind == TCP_OPTION_END_OF_OPTION){
            option_dsc.length = 1;
            option_dsc.ptr = nullptr;
        }else{
            option_dsc.length = *(startPtr+1);
            option_dsc.ptr = startPtr + 2;
        }
        startPtr += option_dsc.length;
        this->options_dsc.insert(index++,option_dsc);
        headLenCount += option_dsc.length;
        if(option_dsc.kind == TCP_OPTION_END_OF_OPTION)
            break;
    }
}

qint32 DissectResultTcp::getOptionIndex(enum TCP_OPTION option){
    for(int i = 0; i < this->options_dsc.keys().length(); i++)
        if(this->options_dsc.value(i).kind == option)
            return i;
    return -1;
}
