#include "udp.h"

Udp::Udp(ProTree *proTree,tcp_ip_protocol_family::DissectResultUdp *dissectResultUdp,void *reserves)
{
    Q_UNUSED(dissectResultUdp)
    Q_UNUSED(reserves)

    quint64 option = DissectResultBase::GetDissectorOptionPtr()->value("udp");

    qint32 start = dissectResultUdp->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("udp");
    //qint32 tempStart;

    UdpInfo *udpInfo = (UdpInfo*)dissectResultUdp->GetDissectResultBase()->GetAdditionalPtr(UDP_INFO_PTR);

    // Summery
    proTree->AddItem("udp"
                     ,QString("User Datagram Protocol, Src port: %1, Dst port: %2")
                     .arg(dissectResultUdp->GetSourcePort())
                     .arg(dissectResultUdp->GetDestinationPort())
                     ,start
                     ,dissectResultUdp->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("udp") - 1
                     ,ProTree::level::CURRENT
                     );

    // Summery - Source Port
    proTree->AddNextLayerItem("udp"
                              ,QString("Source Port: %1")
                              .arg(dissectResultUdp->GetSourcePort())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultUdp::TRANSPORTLAYER_UDP_FIELD_LENGTH_SOURCE_PORT
                              ,true);

    // Summery - Destination Port
    proTree->AddCurrentLayerItem("udp"
                                 ,QString("Destination Port: %1")
                                 .arg(dissectResultUdp->GetDestinationPort())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultUdp::TRANSPORTLAYER_UDP_FIELD_LENGTH_DESTINATION_PORT
                                 ,true);

    // Summery - Length
    proTree->AddCurrentLayerItem("udp"
                                 ,QString("Length: %1")
                                 .arg(dissectResultUdp->GetLength())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultUdp::TRANSPORTLAYER_UDP_FIELD_LENGTH_LENGTH
                                 ,true);

    // Summery - Checksum
    proTree->AddCurrentLayerItem("udp"
                                 ,QString("Checksum: %1")
                                 .arg(dissectResultUdp->GetChecksumStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultUdp::TRANSPORTLAYER_UDP_FIELD_LENGTH_CHECKSUM
                                 ,true);

    //Summery - Calculated Checksum
    if( option & UDP_VALIDATE_CHECKSUM ){
        proTree->AddItem("udp"
                        ,QString("[Checksum Status: %1]")
                        .arg(udpInfo->badChecksum ? "Bad" : "Good")
                        ,ProTree::level::CURRENT);
        proTree->AddItem("udp"
                         ,QString("[Calculated Checksum: %1]")
                         .arg(dissectResultUdp->GetCalculatedChecksum())
                         ,ProTree::level::CURRENT);
    }

    // Summery - StreamIndex
    proTree->AddItem("udp"
                     ,QString("[Stream Index: %1]")
                     .arg(dissectResultUdp->GetStreamIndex())
                     ,ProTree::level::CURRENT);

    proTree->Pop();
}
