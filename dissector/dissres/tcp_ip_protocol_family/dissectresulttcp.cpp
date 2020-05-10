#include "dissectresulttcp.h"
#include <QtDebug>

using namespace tcp_ip_protocol_family;

//StreamTcp DissectResultTcp::stream;

StreamTcp2 DissectResultTcp::stream2;

QHash<qint32,QString> DissectResultTcp::tcp_option_val =
{
    {TCP_OPTION_END_OF_OPTION,""}
    ,{TCP_OPTION_NO_OPERATION,"No-operation"}
    ,{TCP_OPTION_MAXIMUM_SEGMENT_SIZE,"Maximum segment size"}
    ,{TCP_OPTION_WINDOW_SCALE_OPTION,"Window scale"}
    ,{TCP_OPTION_SACK_PERMITTED_OPTION,"Sack permitted"}
    ,{TCP_OPTION_SACK_OPTION_FORMAT,"SACK"}
    ,{TCP_OPTION_TIMESTAMPS_OPTION,"Timestamps"}
};

DissectResultTcp::DissectResultTcp(DissectResultBase *dissectResultBase,void *reserves):DissectResult(dissectResultBase)
{
    Q_UNUSED(reserves)
    quint64 options = DissectResultBase::GetDissectorOptionPtr()->value("tcp");
    TcpInfo *tcpInfo_ptr = new TcpInfo;
    dissectResultBase->AddAdditional(TCP_INFO_PTR,(void*)tcpInfo_ptr);

    dissectResultBase->PushToProtocolList("tcp",TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("tcp");

    dissectResultBase->UpdateProtocolList("tcp",this->GetOffset() * 4);

    this->dealTcpOptions();


    tcpInfo_ptr->SYN = this->SYN();
    tcpInfo_ptr->FIN = this->FIN();
    tcpInfo_ptr->RST = this->RST();
    tcpInfo_ptr->ACK = this->ACK();
    tcpInfo_ptr->ack = this->GetAck();
    tcpInfo_ptr->seq = this->GetSeq();
    tcpInfo_ptr->segLen = this->GetPayloadLen();
    tcpInfo_ptr->totalLen = this->GetTotalLen();
    tcpInfo_ptr->windowVal = this->GetWindow();
    tcpInfo_ptr->windowSclae = this->GetOptionWindowScale();
    tcpInfo_ptr->time.tv_sec = this->GetDissectResultBase()->GetPkthdr()->ts.tv_sec;
    tcpInfo_ptr->time.tv_usec = this->GetDissectResultBase()->GetPkthdr()->ts.tv_usec;
    tcpInfo_ptr->echoReplayTime.tv_sec = (__time_t)this->GetOptionTimestampEchoReply();
    tcpInfo_ptr->echoReplayTime.tv_usec = 0;



    if( this->GetDissectResultBase()->ContainProtocol("ipv4") ){
        Ipv4Info *ipv4Info = (Ipv4Info*)this->GetDissectResultBase()->GetAdditionalPtr(IPV4_INFO_PTR);
        this->SetStremIndexPlusOne( stream2.AddWithWindow(dissectResultBase
                             ,ipv4Info->src
                             ,ipv4Info->dst
                             ,ipv4Info->addrLen
                             ,this->GetSourcePortPtr()
                             ,this->GetDestinationPortPtr()
                             ,TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT
                             ));
    }else{
        this->SetStremIndexPlusOne(0);
    }


    tcpInfo_ptr->streamPlusOne = this->GetStreamIndexPlusOne();

    if( options & TCP_VALIDATE_CHECKSUM ){
        Checksum2 checksum(this->GetDissectResultBase(),"tcp");
        this->calculatedChecksumStr.append(checksum.GetCalculateChecksumStr());
        if( this->calculatedChecksumStr != this->GetChecksumStr() )
            tcpInfo_ptr->badChecksum = true;
    }

    /*设置Summery*/
    QString status = this->GetSegmentStatusStr();
    dissectResultBase->SetSummery(QString("%1").arg(status));

    dissectResultBase->AppendSummery(QString("%1 -> %2").arg(this->GetSourcePort()).arg(this->GetDestinationPort()));

    QString flag;
    if(this->SYN())
        flag.append("SYN,");
    if(this->ACK())
        flag.append("ACK,");
    if(this->RST())
        flag.append("RST,");
    if(this->FIN())
        flag.append("FIN,");
    flag.insert(0,"[");
    flag.remove(flag.length() - 1,1);
    flag.append("]");
    dissectResultBase->AppendSummery(QString("%1").arg(flag));

    dissectResultBase->AppendSummery(QString("seq=%1").arg(this->GetRelativeSeq()));
    dissectResultBase->AppendSummery(QString("ack=%1").arg(this->GetRelativeAck()));
    dissectResultBase->AppendSummery(QString("len=%1").arg(this->GetPayloadLen()));
    dissectResultBase->AppendSummery(QString("win=%1").arg(this->GetCalculatedWindow()));

    if( this->GetOptionTimestampValue() != 0 && !(options & TCP_IGNORE_TIMESTAMPS_IN_SUMMERY)){
        dissectResultBase->AppendSummery(QString("TSval=%1").arg(this->GetOptionTimestampValue()));
        dissectResultBase->AppendSummery(QString("TSecr=%1").arg(this->GetOptionTimestampEchoReply()));
    }

    if( tcpInfo_ptr->badChecksum && (options & TCP_VALIDATE_CHECKSUM))
        dissectResultBase->AppendSummery("[Bad Checksum]");

    if(header != nullptr)
        this->addNextLayer(dissectResultBase,reserves);
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

quint16 DissectResultTcp::GetServPort(){
    return this->GetSourcePort() > this->GetDestinationPort() ? this->GetDestinationPort() : this->GetSourcePort();
}

/*处理Seq和Ack*/
quint32 DissectResultTcp::GetSeq(){
    return ntohl(*(quint32*)this->header->seq);
}

quint32 DissectResultTcp::GetRelativeSeq(){
    return this->GetSeq() - DissectResultTcp::stream2.GetBaseSeq(this->GetStreamIndexPlusOne());
}

quint32 DissectResultTcp::GetAck(){
    return ntohl(*(quint32*)this->header->ack);
}

quint32 DissectResultTcp::GetRelativeAck(){
    if( this->SYN() && !this->ACK() )
        return this->GetAck();
    return this->GetAck() - DissectResultTcp::stream2.GetBaseSeq(-this->GetStreamIndexPlusOne());
}

/*处理首部长度和负载长度*/
quint8 DissectResultTcp::GetOffset(){
    return (this->header->offset_reserved_flags[0] & 0xf0) >> 4;
}

QString DissectResultTcp::GetOffsetDotStr(){
    return Bit::GetDotStrFormOctetPtr(this->header->offset_reserved_flags,4,7);
}

quint32 DissectResultTcp::GetPayloadLen(){
    if( this->GetDissectResultBase()->ContainProtocol("ipv4")){
        Ipv4Info *ipv4Info = (Ipv4Info*)this->GetDissectResultBase()->GetAdditionalPtr(IPV4_INFO_PTR);
        return ipv4Info->payloadLen - this->GetOffset() * 4;
    }else{
        return 0;
    }
}

quint32 DissectResultTcp::GetTotalLen(){
    if( this->GetDissectResultBase()->ContainProtocol("ipv4")){
        Ipv4Info *ipv4Info = (Ipv4Info*)this->GetDissectResultBase()->GetAdditionalPtr(IPV4_INFO_PTR);
        return ipv4Info->payloadLen;
    }else{
        return 0;
    }
}

/*Flags*/

/* ---------------------------------------------------------------
* Data  |Reserved   |U|A|P|R|S|F|   Window                      |
* Offset|           |R|C|S|S|Y|I|                               |
*       |           |G|K|H|T|N|N|                               |
* ---------------------------------------------------------------
*/
quint8 DissectResultTcp::GetReserved(){
    return (*this->header->offset_reserved_flags & 0x0e) >> 1;
}

QString DissectResultTcp::GetReservedStr(){
    return QString("%1%2%3")
            .arg(Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags,3))
            .arg(Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags,2))
            .arg(Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags,1));
}

bool DissectResultTcp::RESERVED(){
    if( this->GetReserved() )
        return true;
    return false;
}

bool DissectResultTcp::NONCE(){
    if( Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags,0) )
        return true;
    return false;
}

bool DissectResultTcp::CWR(){  //Congestion Window Resuced
    if( Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags + 1,7) )
        return true;
    return false;
}

bool DissectResultTcp::ECN_ECHO(){
    if( Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags + 1,6) )
        return true;
    return false;
}

bool DissectResultTcp::URG(){
    if( Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags + 1,5) )
        return true;
    return false;
}

bool DissectResultTcp::ACK(){
    if( ((this->header->offset_reserved_flags[1] & 0x10) >> 4) == 1 )
        return true;
    else
        return false;
}

bool DissectResultTcp::PSH(){
    if( Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags + 1,3) )
        return true;
    return false;
}

bool DissectResultTcp::RST(){
    if(((this->header->offset_reserved_flags[1] & 0x04) >> 2) == 1)
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

bool DissectResultTcp::FIN(){
    if(((this->header->offset_reserved_flags[1] & 0x01) >> 0) == 1)
        return true;
    else
        return false;
}

QString DissectResultTcp::GetFlagsStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->offset_reserved_flags,2).remove(2,1);
}

QString DissectResultTcp::GetFlagMeanings(){
    QString meanings;
    if( this->SYN() )
        meanings.append("SYN, ");
    if( this->ACK() )
        meanings.append("ACK, ");
    if( this->RST() )
        meanings.append("RST, ");
    if( this->FIN() )
        meanings.append("FIN, ");
    if( this->URG() )
        meanings.append("URG, ");
    if( this->PSH() )
        meanings.append("PSH, ");
    if( this->NONCE() )
        meanings.append("Nonce, ");
    if( this->CWR() )
        meanings.append("CED, ");
    if( this->ECN_ECHO() )
        meanings.append("ECN-ECHO, ");
    return meanings.remove(meanings.length() -2,2);
}

QString DissectResultTcp::GetFlagDotMeanings(){
    return QString("%1%2%3%4%5%6%7%8%9%10%11%12")
            .arg(Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags,3) ? "R" : ".")
            .arg(Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags,2) ? "R" : ".")
            .arg(Bit::GetBitFromOctetPtr(this->header->offset_reserved_flags,1) ? "R" : ".")
            .arg(this->NONCE() ? "N" : ".")
            .arg(this->CWR() ? "C" : ".")
            .arg(this->ECN_ECHO() ? "E" : ".")
            .arg(this->URG() ? "U" : ".")
            .arg(this->ACK() ? "A" : ".")
            .arg(this->PSH() ? "P" : ".")
            .arg(this->RST() ? "R" : ".")
            .arg(this->SYN() ? "S" : ".")
            .arg(this->FIN() ? "F" : ".");
}

/*Window*/
quint8* DissectResultTcp::GetWindowPtr(){
    return this->header->window;
}

quint16 DissectResultTcp::GetWindow(){
    return ntohs(*(quint16*)this->header->window);
}

quint32 DissectResultTcp::GetCalculatedWindow(){
    return this->GetWindow() * DissectResultTcp::stream2.GetWindowMultiplier(this->GetStreamIndexPlusOne());
}

/*Checksum*/
QString DissectResultTcp::GetChecksumStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->checksum,TRANSPORTLAYER_TCP_FIELD_LENGTH_CHECKSUM);
}

QString DissectResultTcp::GetCalculatedChecksumStr(){
    return this->calculatedChecksumStr;
}

/*Urgent Point*/
quint16 DissectResultTcp::GetUrgentPoint(){
    return ntohs(*(quint16*)this->header->urgentPoint);
}

/*
 * Tcp options
 * 返回-1表示没有使用
 */
//qint32 DissectResultTcp::GetOptionPtrByIndex(quint8 *kind,quint8 *length, const quint8 **ptr, qint32 index){
//    if(this->options_dsc.isEmpty() || !this->options_dsc.contains(index))
//        return -1;
//    *kind = this->options_dsc.value(index).kind;
//    *length = this->options_dsc.value(index).length;
//    *ptr = this->options_dsc.value(index).ptr;
//    return 1;
//}

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

quint32 DissectResultTcp::GetOptionTimestampValue(){
    qint32 index = this->getOptionIndex(TCP_OPTION_TIMESTAMPS_OPTION);
    if(index != -1)
        return ntohl(*(quint32*)this->options_dsc.value(index).ptr);
    return 0;
}

quint32 DissectResultTcp::GetOptionTimestampEchoReply(){
    qint32 index = this->getOptionIndex(TCP_OPTION_TIMESTAMPS_OPTION);
    if(index != -1)
        return ntohl(*(quint32*)(this->options_dsc.value(index).ptr + 4));
    return 0;
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
            sacks.append(ntohl(ptr[i]));
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
            sacks.append(ntohl(ptr[i]) - stream2.GetBaseSeq(-this->GetStreamIndexPlusOne()));
    }
    return sacks;
}

qint32 DissectResultTcp::GetOptionsCount(){
    return this->options_dsc.keys().length();
}

qint32 DissectResultTcp::GetOptionTypeByIndex(qint32 index){
    return this->options_dsc.value(index).kind;
}

qint32 DissectResultTcp::GetOptionLenByIndex(qint32 index){
    return this->options_dsc.value(index).length;
}

qint32 DissectResultTcp::GetOptionsLen(){
    qint32 len = 0;
    for( qint32 index = 0; index < this->GetOptionsCount(); index++ ){
        if( this->GetOptionTypeByIndex(index) != TCP_OPTION_END_OF_OPTION )
            len += this->GetOptionLenByIndex(index);
    }
    return len;
}

QString DissectResultTcp::GetOptionsSummery(){
    QString summery;
    for( qint32 index = 0; index < this->GetOptionsCount(); index++ ){
        if( this->GetOptionTypeByIndex(index) != TCP_OPTION_END_OF_OPTION )
            summery.append(DissectResultTcp::tcp_option_val.value(this->GetOptionTypeByIndex(index)) + ", ");
    }
    if( !summery.isEmpty() )
        return summery.remove(summery.length() -2,2);
    else
        return summery;
}
/*
* 从DissectResultBase的保留字段获取数据，
*/

/*分析Seq/Ack*/
QString DissectResultTcp::GetSegmentStatusStr(){
    qint32 status = ((TcpInfo*)this->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR))->status;
    QString str = "";

    if( status & TCP_A_ZERO_WINDOW_PROBE )
        str.append(tcp_segment_status_vals.value(TCP_A_ZERO_WINDOW_PROBE));

    if( status & TCP_A_ZERO_WINDOW )
        str.append(tcp_segment_status_vals.value(TCP_A_ZERO_WINDOW));

    if( status & TCP_A_LOST_PACKET )
        str.append(tcp_segment_status_vals.value(TCP_A_LOST_PACKET));

    if( status & TCP_A_KEEP_ALIVE )
        str.append(tcp_segment_status_vals.value(TCP_A_KEEP_ALIVE));

    if( status & TCP_A_WINDOW_UPDATE )
        str.append(tcp_segment_status_vals.value(TCP_A_WINDOW_UPDATE));

    if( status & TCP_A_WINDOW_FULL )
        str.append(tcp_segment_status_vals.value(TCP_A_WINDOW_FULL));

    if( status & TCP_A_KEEP_ALIVE_ACK )
        str.append(tcp_segment_status_vals.value(TCP_A_KEEP_ALIVE_ACK));

    TcpInfo *tcpInfo = (TcpInfo*)this->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR);
    if ( (status & TCP_A_DUPLICATE_ACK) && !(status & TCP_A_KEEP_ALIVE_ACK) && !(status & TCP_A_KEEP_ALIVE) ){
        str.append(tcp_segment_status_vals.value(TCP_A_DUPLICATE_ACK)
                   .arg(tcpInfo->dupack_frame)
                   .arg(tcpInfo->dupack_num));
    }

    if( status & TCP_A_ACK_LOST_PACKET ){
        str.append(tcp_segment_status_vals.value(TCP_A_ACK_LOST_PACKET));
    }

    if( status & TCP_A_FAST_RETRANSMISSION ){
        str.append(tcp_segment_status_vals.value(TCP_A_FAST_RETRANSMISSION));
    }

    if( status & TCP_A_OUT_OF_ORDER ){
        str.append(tcp_segment_status_vals.value(TCP_A_OUT_OF_ORDER));
    }

    if(status & TCP_A_SPURIOUS_RETRANSMISSION ){
        str.append(tcp_segment_status_vals.value(TCP_A_SPURIOUS_RETRANSMISSION));
    }

    if( status & TCP_A_RETRANSMISSION ){
        str.append(tcp_segment_status_vals.value(TCP_A_RETRANSMISSION));
    }


    return str;
}

/*
*取得分片的前一分片的包Index，若无前一个分片，或此分片长度为0，返回-1
*/
qint64 DissectResultTcp::GetPrevious(){
    //return this->GetDissectResultBase()->GetAdditionalVal(TCP_PRE_SEGMENT);
    return 0;
}

Stream& DissectResultTcp::GetStreamRecorder(){
    return DissectResultTcp::stream2;
}

StreamTcp2& DissectResultTcp::GetStreamTcp2Recorder(){
    return DissectResultTcp::stream2;
}

/*Private*/

void DissectResultTcp::addNextLayer(DissectResultBase *dissectResultBase,void *reserves){
//    quint16 srcPort = this->GetSourcePort();
//    quint16 dstPort = this->GetDestinationPort();
    quint32 segLen = this->GetPayloadLen();
//    quint16 servPort = srcPort < dstPort ? srcPort : dstPort;

    if( segLen > 0 ){
        switch (this->GetServPort()) {
        case TRANSPORTLAYER_TCP_SERV::HTTP:
        {
            this->SetNextLayer( (void*)(new DissectResultHttp(dissectResultBase,reserves)) );
            break;
        }
        default:
            this->GetDissectResultBase()->SetSummery(this->GetDissectResultBase()->GetSummery().append(QString("未处理用用层类型: %1")
                                                .arg(this->GetServPort())
                                                ));
            this->SetNextLayer();
            break;
        }
    }
}

/*Tcp Options*/
void DissectResultTcp::dealTcpOptions(){
    if( this->GetOffset() * 4 == TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH ){
        struct option_dsc_t option_dsc;
        option_dsc.length = 1;
        option_dsc.kind = TCP_OPTION_END_OF_OPTION;
        option_dsc.ptr = nullptr;
        this->options_dsc.insert(0,option_dsc);
        return;
    }

    const quint8 *startPtr = this->GetDissectResultBase()->GetProtocolHeaderStartPtrByName("tcp") + TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH;
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

qint32 DissectResultTcp::getOptionIndex(qint32 option){
    for(int i = 0; i < this->options_dsc.keys().length(); i++)
        if(this->options_dsc.value(i).kind == option)
            return i;
    return -1;
}
