#include "dissectresulttcp.h"
#include <QtDebug>

using namespace tcp_ip_protocol_family;

//StreamTcp DissectResultTcp::stream;

StreamTcp2 DissectResultTcp::stream2;

DissectResultTcp::DissectResultTcp(DissectResultBase *dissectResultBase)
{
    this->dissectResultBase = dissectResultBase;

    dissectResultBase->PushToProtocolList("tcp",TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("tcp");

    dissectResultBase->UpdateProtocolList("tcp",this->GetOffset() * 4);

    this->dealTcpOptions();

    dissectResultBase->AddAdditional(TCP_ISSYN,this->SYN()?1:0);
    dissectResultBase->AddAdditional(TCP_ISRST,this->RST()?1:0);
    dissectResultBase->AddAdditional(TCP_ISFIN,this->FIN()?1:0);
    dissectResultBase->AddAdditional(TCP_ISACK,this->ACK()?1:0);
    dissectResultBase->AddAdditional(TCP_SEQ_VAL,this->GetSeq());
    dissectResultBase->AddAdditional(TCP_ACK_VAL,this->GetAck());
    dissectResultBase->AddAdditional(TCP_PAYLOAD_LEN,this->GetPayloadLen());
    dissectResultBase->AddAdditional(TCP_WINDOW,this->GetWindow());
    dissectResultBase->AddAdditional(TCP_WINDOW_MULTIPLIER,this->GetOptionWindowMultiplier());
//    this->streamIndexPlusOne = stream.AddWithWindow(dissectResultBase
//                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_SOURCE_ADDRESS_PTR)
//                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_DESTINATION_ADDRESS_PTR)
//                         ,dissectResultBase->GetAdditionalVal(IP_ADDRESS_LENGTH)
//                         ,this->GetSourcePortPtr()
//                         ,this->GetDestinationPortPtr()
//                         ,TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT
//                         );
    this->streamIndexPlusOne = stream2.AddWithWindow(dissectResultBase
                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_SOURCE_ADDRESS_PTR)
                         ,(quint8*)dissectResultBase->GetAdditionalPtr(IP_DESTINATION_ADDRESS_PTR)
                         ,dissectResultBase->GetAdditionalVal(IP_ADDRESS_LENGTH)
                         ,this->GetSourcePortPtr()
                         ,this->GetDestinationPortPtr()
                         ,TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT
                         );
    dissectResultBase->SetSummery(QString("%1 -> %2").arg(this->GetSourcePort()).arg(this->GetDestinationPort()));
    dissectResultBase->SetSummery(QString("%1 Window scale:%2 maxSeg:%3 tval:%4 ter:%5 %6 win:%7 c-win:%8")
                                  .arg(this->GetSegmentStatusStr())
                                  .arg(this->GetOptionWindowScale())
                                  .arg(this->GetOptionMaximumSegmentSize())
                                  .arg(this->GetOptionTimestampValue())
                                  .arg(this->GetOptionTimestampEchoReply())
                                  .arg(this->SYN()?"SYN":"")
                                  .arg(this->GetWindow())
                                  .arg(this->GetCalculatedWindow())
                                  );
    QString flag;
    if(this->SYN())
        flag.append("SYN-");
    if(this->ACK())
        flag.append("ACK-");
    if(this->RST())
        flag.append("RST-");
    if(this->FIN())
        flag.append("FIN");
    dissectResultBase->SetSummery(QString("%1 %2 %3 -> %4 str:%5 r-seq:%6 seq:%7")
                                       .arg(this->GetSegmentStatusStr())
                                       .arg(flag)
                                       .arg(this->GetSourcePort())
                                       .arg(this->GetDestinationPort())
                                       .arg(this->GetOriginalStreamIndex())
                                       .arg(this->GetRelativeSeq())
                                       .arg(this->GetSeq())
                                       //.arg(this->GetRelativeSeq() + this->GetPayloadLen())
                                       //.arg(this->ACK() ? this->GetRelativeAck() : 0)
                                       //.arg(this->GetWindow())
                                  );
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
    if( DissectResultTcp::stream2.GetBaseSeq(streamIndexPlusOne) == 0)
        qDebug() << "啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊";
    return this->GetSeq() - DissectResultTcp::stream2.GetBaseSeq(this->streamIndexPlusOne);
}

quint32 DissectResultTcp::GetAck(){
    return ntohl(*(quint32*)this->header->ack);
}

quint32 DissectResultTcp::GetRelativeAck(){
    return this->GetAck() - DissectResultTcp::stream2.GetBaseSeq(-this->streamIndexPlusOne);
}

/*处理首部长度和负载长度*/
quint8 DissectResultTcp::GetOffset(){
    return (this->header->offset_reserved_flags[0] & 0xf0) >> 4;
}

quint32 DissectResultTcp::GetPayloadLen(){
    return this->dissectResultBase->GetAdditionalVal(TCP_TOTAL_LEN) - this->GetOffset() * 4;
}

/*Flags*/

/* ---------------------------------------------------------------
* Data  |Reserved   |U|A|P|R|S|F|   Window                      |
* Offset|           |R|C|S|S|Y|I|                               |
*       |           |G|K|H|T|N|N|                               |
* ---------------------------------------------------------------
*/
bool DissectResultTcp::ACK(){
    if(((this->header->offset_reserved_flags[1] & 0x10) >> 4) == 1)
        return true;
    else
        return false;
}

bool DissectResultTcp::SYN(){
    if(((this->header->offset_reserved_flags[1] & 0x02) >> 1) == 1)
        return true;
    else
        return false;
}

bool DissectResultTcp::RST(){
    if(((this->header->offset_reserved_flags[1] & 0x04) >> 2) == 1)
        return true;
    else
        return false;
}

bool DissectResultTcp::FIN(){
    if(((this->header->offset_reserved_flags[1] & 0x01) >> 0) == 1)
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
    return this->GetWindow() * stream2.GetWindowMultiplier(streamIndexPlusOne);
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

qint8 DissectResultTcp::GetOptionSackPermitted(){
    qint32 index = this->getOptionIndex(TCP_OPTION_SACK_PERMITTED_OPTION);
    if(index != -1)
        return 1;
    return -1;
}

QList<quint32> DissectResultTcp::GetOptionSacks(){
    qint32 index = this->getOptionIndex(TCP_OPTION_SACK_OPTION_FORMAT);
    QList<quint32> sacks;
    if(index != -1){
        quint8 length = this->options_dsc.value(index).length;
        const quint32 *ptr = (quint32*)this->options_dsc.value(index).ptr;
        for(quint8 i = 0; i < (length -2)/4; i++)
            sacks.append(ptr[i]);
    }
    return sacks;
}

QList<quint32> DissectResultTcp::GetOptionRelativeSacks(){
    qint32 index = this->getOptionIndex(TCP_OPTION_SACK_OPTION_FORMAT);
    QList<quint32> sacks;
    if(index != -1){
        quint8 length = this->options_dsc.value(index).length;
        const quint32 *ptr = (quint32*)this->options_dsc.value(index).ptr;
        for(quint8 i = 0; i < (length -2)/4; i++)
            sacks.append(ptr[i] - stream2.GetBaseSeq(this->streamIndexPlusOne));
    }
    return sacks;
}

/*
* 从DissectResultBase的保留字段获取数据，
*/

//#define TCP_A_RETRANSMISSION  0x0001
//#define TCP_A_LOST_PACKET  0x0002
//#define TCP_A_ACK_LOST_PACKET 0x0004
//#define TCP_A_KEEP_ALIVE  0x0008
//#define TCP_A_DUPLICATE_ACK  0x0010
//#define TCP_A_ZERO_WINDOW  0x0020
//#define TCP_A_ZERO_WINDOW_PROBE  0x0040
//#define TCP_A_ZERO_WINDOW_PROBE_ACK  0x0080
//#define TCP_A_KEEP_ALIVE_ACK  0x0100
//#define TCP_A_OUT_OF_ORDER  0x0200
//#define TCP_A_FAST_RETRANSMISSION  0x0400
//#define TCP_A_WINDOW_UPDATE  0x0800
//#define TCP_A_WINDOW_FULL  0x1000
//#define TCP_A_REUSED_PORTS  0x2000
//#define TCP_A_SPURIOUS_RETRANSMISSION  0x4000
//#define TCP_A_PREVIOUS_SEGMENT_NOT_CAPTURED 0x8000


/*分析Seq/Ack*/
QString DissectResultTcp::GetSegmentStatusStr(){
    qint32 status = this->dissectResultBase->GetAdditionalVal(TCP_STATUS2);
    QString str = "";

    if( status & TCP_A_ZERO_WINDOW_PROBE )
        str.append(tcp_segment_status_vals.value(TCP_A_ZERO_WINDOW_PROBE)).append("-");

    if( status & TCP_A_ZERO_WINDOW )
        str.append(tcp_segment_status_vals.value(TCP_A_ZERO_WINDOW)).append("-");

    if( status & TCP_A_LOST_PACKET )
        str.append(tcp_segment_status_vals.value(TCP_A_LOST_PACKET)).append("-");

    if( status & TCP_A_KEEP_ALIVE )
        str.append(tcp_segment_status_vals.value(TCP_A_KEEP_ALIVE)).append("-");

    if( status & TCP_A_WINDOW_UPDATE )
        str.append(tcp_segment_status_vals.value(TCP_A_WINDOW_UPDATE)).append("-");

    if( status & TCP_A_WINDOW_FULL )
        str.append(tcp_segment_status_vals.value(TCP_A_WINDOW_FULL)).append("-");

    if( status & TCP_A_KEEP_ALIVE_ACK )
        str.append(tcp_segment_status_vals.value(TCP_A_KEEP_ALIVE_ACK)).append("-");

    return str;
}

/*
*取得分片的前一分片的包Index，若无前一个分片，或此分片长度为0，返回-1
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
