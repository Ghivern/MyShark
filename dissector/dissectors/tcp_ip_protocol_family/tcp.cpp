#include "tcp.h"

Tcp::Tcp(ProTree *proTree,tcp_ip_protocol_family::DissectResultTcp *dissectResultTcp,void *reserves)
{
    Q_UNUSED(dissectResultTcp)
    Q_UNUSED(reserves)
    quint64 option = DissectResultBase::GetDissectorOptionPtr()->value("tcp");

    bool rSeq = (option & TCP_ANALYZE_TCP_SEQUENCE_NUMBER) || (option & TCP_RELATIVE_SEQUENCE_NUMBER);

    qint32 start = dissectResultTcp->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("tcp");

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
                         ,ProTree::level::CURRENT);
    }

}
