#include "tcp.h"

Tcp::Tcp(ProTree *proTree,tcp_ip_protocol_family::DissectResultTcp *dissectResultTcp,void *reserves)
{
    Q_UNUSED(dissectResultTcp)
    Q_UNUSED(reserves)
    quint64 option = DissectResultBase::GetDissectorOptionPtr()->value("tcp");

    bool rSeq = (option & TCP_ANALYZE_TCP_SEQUENCE_NUMBER) || (option & TCP_RELATIVE_SEQUENCE_NUMBER);

    qint32 start = dissectResultTcp->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("tcp");
    qint32 tempStart;
    TcpInfo *tcpInfo = (TcpInfo*)dissectResultTcp->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR);

    // Summery
    proTree->AddItem("tcp"
                     ,QString("Transmission Control Protocol, Src Port: %1, Dst Port: %2, Seq:%3, Ack:%4, Len:%5")
                     .arg(dissectResultTcp->GetSourcePort())
                     .arg(dissectResultTcp->GetDestinationPort())
                     .arg(rSeq ? dissectResultTcp->GetRelativeSeq() : dissectResultTcp->GetSeq())
                     .arg(rSeq ? dissectResultTcp->GetRelativeAck() : dissectResultTcp->GetAck())
                     .arg(dissectResultTcp->GetPayloadLen())
                     ,start
                     ,dissectResultTcp->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("tcp") - 1
                     ,ProTree::level::CURRENT);

    // Summery - Source Port
    proTree->AddNextLayerItem("tcp"
                              ,QString("Source Port: %1")
                              .arg(dissectResultTcp->GetSourcePort())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT
                              ,true);

    // Summery - Destination Port
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("Destination Port: %2")
                                 .arg(dissectResultTcp->GetDestinationPort())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_DESTINATION_PORT
                                 ,true);

    // Summery - Stream Index
    if( option & TCP_ANALYZE_TCP_SEQUENCE_NUMBER ){
        proTree->AddItem("tcp"
                         ,QString("[Stream Index: %1]")
                         .arg(dissectResultTcp->GetStreamIndex())
                         ,ProTree::level::CURRENT
                         );
    }

    // Summery - Tcp Segment Len
    tempStart = start
                + tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_SEQ
                + tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_ACK;
    proTree->AddItem("tcp"
                     ,QString("[TCP segment Len: %1]")
                     .arg(dissectResultTcp->GetPayloadLen())
                     ,tempStart
                     ,tempStart
                     ,ProTree::level::CURRENT
                     );

    // Summery - Sequence number
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("Sequence number: %1 %2")
                                 .arg(rSeq ? dissectResultTcp->GetRelativeSeq() : dissectResultTcp->GetSeq())
                                 .arg(rSeq ? "(relative sequence number)" : "")
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_SEQ
                                 ,true);

    // Summery - Next Sequence number
    proTree->AddItem("tcp"
                    ,QString("[Next Sequence number: %1 %2]")
                    .arg((rSeq ? dissectResultTcp->GetRelativeSeq() : dissectResultTcp->GetSeq()) + dissectResultTcp->GetPayloadLen())
                    .arg(rSeq ? "(relative sequence number)" : "")
                    );

    // Summery - Acknowledgment number
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("Acknowledgment number: %1 %2")
                                 .arg( rSeq ? dissectResultTcp->GetRelativeAck() : dissectResultTcp->GetAck() )
                                 .arg( rSeq && dissectResultTcp->ACK() ? "(relative ack number)" : "" )
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_ACK
                                 ,true);

    // Summery - Header length
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("%1 = Header length: %2 bytes (%3)")
                                 .arg(dissectResultTcp->GetOffsetDotStr())
                                 .arg(dissectResultTcp->GetOffset() * 4)
                                 .arg(dissectResultTcp->GetOffset())
                                 ,&start
                                 ,1
                                 ,false);

    // Summery - Flags
    tempStart = start;
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("Flags: %1 (%2)")
                                 .arg(dissectResultTcp->GetFlagsStr())
                                 .arg(dissectResultTcp->GetFlagMeanings())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_DATAOFFSET_RESERVED_FLAGS
                                 ,false);
    proTree->AddNextLayerItem("tcp"
                              ,QString("%1. .... .... = Reserved: %2")
                              .arg(dissectResultTcp->GetReservedStr())
                              .arg(dissectResultTcp->GetReserved() ? "Set" : "Not Set")
                              ,&start
                              ,1
                              ,false);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("...%1 .... .... = Nonce: %2")
                                 .arg(dissectResultTcp->NONCE() ? 1 : 0)
                                 .arg(dissectResultTcp->NONCE() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,true);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... %1... .... = Congestion Window Reduced(CWR): %2")
                                 .arg(dissectResultTcp->CWR() ? 1 : 0)
                                 .arg(dissectResultTcp->CWR() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,false);

    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... .%1.. .... = ECN-Echo: %2")
                                 .arg(dissectResultTcp->ECN_ECHO() ? 1 : 0)
                                 .arg(dissectResultTcp->ECN_ECHO() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,false);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... ..%1. .... = Urgent: %2")
                                 .arg(dissectResultTcp->URG() ? 1 : 0)
                                 .arg(dissectResultTcp->URG() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,false);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... ...%1 .... = Acknowledgement: %2")
                                 .arg(dissectResultTcp->ACK() ? 1 : 0)
                                 .arg(dissectResultTcp->ACK() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,false);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... .... %1... = Push: %2")
                                 .arg(dissectResultTcp->PSH() ? 1 : 0)
                                 .arg(dissectResultTcp->PSH() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,false);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... .... .%1.. = Reset: %2")
                                 .arg(dissectResultTcp->RST() ? 1 : 0)
                                 .arg(dissectResultTcp->RST() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,false);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... .... ..%1. = Syn: %2")
                                 .arg(dissectResultTcp->SYN() ? 1 : 0)
                                 .arg(dissectResultTcp->SYN() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,false);
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString(".... .... ...%1 = Fin: %2")
                                 .arg(dissectResultTcp->FIN() ? 1 : 0)
                                 .arg(dissectResultTcp->FIN() ? "Set" : "Not Set")
                                 ,&start
                                 ,1
                                 ,true);
    proTree->AddItem("tcp"
                     ,QString("[TCP Flags: %1")
                     .arg(dissectResultTcp->GetFlagDotMeanings())
                     ,tempStart
                     ,tempStart + 1
                     ,ProTree::level::CURRENT);

    //Summery - Window Size Value
    proTree->Pop();
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("Window size value: %1")
                                 .arg(dissectResultTcp->GetWindow())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_WINDOW
                                 ,false);

    //Summery - Calculated window size
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("[Calculated window size: %1]")
                                 .arg(dissectResultTcp->GetCalculatedWindow())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_WINDOW
                                 ,true);

    //Summery - Checksum
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("Checksum: %1")
                                 .arg(dissectResultTcp->GetChecksumStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_CHECKSUM
                                 ,true);

    //Summery - Calculated Checksum
    if( option & TCP_VALIDATE_CHECKSUM ){
        proTree->AddItem("tcp"
                         ,QString("[Checksum Status: %1]")
                         .arg(tcpInfo->badChecksum ? "Bad" : "Good")
                         ,ProTree::level::CURRENT);
        proTree->AddItem("tcp"
                         ,QString("[Calculated Checksum: %1]")
                         .arg(dissectResultTcp->GetCalculatedChecksumStr())
                         ,ProTree::level::CURRENT);
    }

    //Summery - Urgent point
    proTree->AddCurrentLayerItem("tcp"
                                 ,QString("Urgent point: %1")
                                 .arg(dissectResultTcp->GetUrgentPoint())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultTcp::TRANSPORTLAYER_TCP_FIELD_LENGTH_URGENTPOINT
                                 ,true);

    //Summery - Options
    if( dissectResultTcp->GetOptionsCount() > 1 ){
        qint32 type = 0;
        QString typeName;
        qint32 len = 0;
        proTree->AddItem("tcp"
                         ,QString("Options: (%1 bytes), %2")
                         .arg(dissectResultTcp->GetOptionsLen())
                         .arg(dissectResultTcp->GetOptionsSummery())
                         ,start
                         ,dissectResultTcp->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("tcp") - 1
                         ,ProTree::level::CURRENT);
        tempStart = start;
        for( qint32 index = 0; index < dissectResultTcp->GetOptionsCount(); index++ ){
            type = dissectResultTcp->GetOptionTypeByIndex(index);
            typeName.clear();
            typeName.append(tcp_ip_protocol_family::DissectResultTcp::tcp_option_val.value(type));
            len = dissectResultTcp->GetOptionLenByIndex(index);
            switch (type) {
            case TCP_OPTION_NO_OPERATION:
            {
                proTree->AddItem("tcp"
                                 ,QString("Tcp option - %1")
                                 .arg(typeName)
                                 ,tempStart
                                 ,tempStart + len - 1
                                 ,index == 0 ? ProTree::NEW : ProTree::CURRENT);
                proTree->AddNextLayerItem("tcp"
                                          ,QString("Kind: %1 (%2)")
                                          .arg(typeName)
                                          .arg(type)
                                          ,&tempStart
                                          ,len
                                          ,true);
                proTree->Pop();
                break;
            }
            case TCP_OPTION_MAXIMUM_SEGMENT_SIZE:
            {
                proTree->AddItem("tcp"
                                 ,QString("Tcp option - %1 : %2 bytes")
                                 .arg(typeName)
                                 .arg(dissectResultTcp->GetOptionMaximumSegmentSize())
                                 ,tempStart
                                 ,tempStart + len - 1
                                 ,index == 0 ? ProTree::NEW : ProTree::CURRENT);
                proTree->AddNextLayerItem("tcp"
                                          ,QString("Kind: %1 (%2)")
                                          .arg(typeName)
                                          .arg(type)
                                          ,&tempStart
                                          ,1
                                          ,true);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("Length: %1")
                                             .arg(len)
                                             ,&tempStart
                                             ,1
                                             ,true);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("MSS Value: %1")
                                             .arg(dissectResultTcp->GetOptionMaximumSegmentSize())
                                             ,&tempStart
                                             ,len - 2
                                             ,true);
                proTree->Pop();

                break;
            }
            case TCP_OPTION_WINDOW_SCALE_OPTION:
            {
                proTree->AddItem("tcp"
                                 ,QString("Tcp option - %1 : %2 (multiply by %3)")
                                 .arg(typeName)
                                 .arg(dissectResultTcp->GetOptionWindowScale())
                                 .arg(dissectResultTcp->GetOptionWindowMultiplier())
                                 ,tempStart
                                 ,tempStart + len - 1
                                 ,index == 0 ? ProTree::NEW : ProTree::CURRENT);
                proTree->AddNextLayerItem("tcp"
                                          ,QString("Kind: %1 (%2)")
                                          .arg(typeName)
                                          .arg(type)
                                          ,&tempStart
                                          ,1
                                          ,true);
                proTree->AddCurrentLayerItem("tcp"
                                          ,QString("Length: %1")
                                          .arg(len)
                                          ,&tempStart
                                          ,1
                                          ,true);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("Shift count: %1")
                                             .arg(dissectResultTcp->GetOptionWindowScale())
                                             ,&tempStart
                                             ,len - 2
                                             ,false);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("Multiply count: %1")
                                             .arg(dissectResultTcp->GetOptionWindowMultiplier())
                                             ,&tempStart
                                             ,len - 2
                                             ,true);
                proTree->Pop();
                break;
            }
            case TCP_OPTION_SACK_PERMITTED_OPTION:
            {
                proTree->AddItem("tcp"
                                 ,QString("Tcp option - %1")
                                 .arg(typeName)
                                 ,tempStart
                                 ,tempStart + len - 1
                                 ,index == 0 ? ProTree::NEW : ProTree::CURRENT);
                proTree->AddNextLayerItem("tcp"
                                          ,QString("Kind: %1 (%2)")
                                          .arg(typeName)
                                          .arg(type)
                                          ,&tempStart
                                          ,1
                                          ,true);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("length: %1")
                                             .arg(len)
                                             ,&tempStart
                                             ,1
                                             ,true);
                proTree->Pop();
                break;
            }
            case TCP_OPTION_SACK_OPTION_FORMAT:
            {
                proTree->AddItem("tcp"
                                 ,QString("Tcp option - %1")
                                 .arg(typeName)
                                 ,tempStart
                                 ,tempStart + len - 1
                                 ,index == 0 ? ProTree::NEW : ProTree::CURRENT);
                proTree->AddNextLayerItem("tcp"
                                          ,QString("Kind: %1 (%2)")
                                          .arg(typeName)
                                          .arg(type)
                                          ,&tempStart
                                          ,1);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("Length: %1")
                                             .arg(len)
                                             ,&tempStart
                                             ,1
                                             ,true);
                for(qint32 index = 0; index < dissectResultTcp->GetOptionSacks().length() - 1; ){
                    proTree->AddCurrentLayerItem("tcp"
                                                 ,QString("left edge = %1 %2")
                                                 .arg(rSeq ? dissectResultTcp->GetOptionRelativeSacks().at(index) : dissectResultTcp->GetOptionSacks().at(index))
                                                 .arg(rSeq ? "(relative)" : "")
                                                 ,&tempStart
                                                 ,4
                                                 ,true
                                                 );
                    proTree->AddCurrentLayerItem("tcp"
                                                 ,QString("right edge = %1 %2")
                                                 .arg(rSeq ? dissectResultTcp->GetOptionRelativeSacks().at(index + 1) : dissectResultTcp->GetOptionSacks().at(index + 1))
                                                 .arg(rSeq ? "(relative)" : "")
                                                 ,&tempStart
                                                 ,4
                                                 ,true
                                                 );
                    index += 2;
                }
                proTree->AddItem("tcp"
                                ,QString("[TCP SACK Count: %1]")
                                .arg(dissectResultTcp->GetOptionSacks().length()/2)
                                ,ProTree::level::CURRENT);
                proTree->Pop();
                break;
            }
            case TCP_OPTION_TIMESTAMPS_OPTION:
            {
                proTree->AddItem("tcp"
                                 ,QString("Tcp option - %1: TSval %2, TSecr %3")
                                 .arg(typeName)
                                 .arg(dissectResultTcp->GetOptionTimestampValue())
                                 .arg(dissectResultTcp->GetOptionTimestampEchoReply())
                                 ,tempStart
                                 ,tempStart + len - 1
                                 ,index == 0 ? ProTree::NEW : ProTree::CURRENT);
                proTree->AddNextLayerItem("tcp"
                                          ,QString("Kind: %1 (%2)")
                                          .arg(typeName)
                                          .arg(type)
                                          ,&tempStart
                                          ,1
                                          ,true);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("Length: %1")
                                             .arg(len)
                                             ,&tempStart
                                             ,1
                                             ,true);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("Timestamp value: %1")
                                             .arg(dissectResultTcp->GetOptionTimestampValue())
                                             ,&tempStart
                                             ,(len - 2)/2
                                             ,true);
                proTree->AddCurrentLayerItem("tcp"
                                             ,QString("Timestamp echo reply: %1")
                                             .arg(dissectResultTcp->GetOptionTimestampEchoReply())
                                             ,&tempStart
                                             ,(len - 2)/2
                                             ,true);
                proTree->Pop();
                break;
            }
            default:
                continue;
            }

        }
        proTree->Pop();
    }


    proTree->Pop();

}
