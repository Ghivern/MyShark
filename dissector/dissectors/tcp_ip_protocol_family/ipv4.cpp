#include "ipv4.h"

Ipv4::Ipv4(ProTree *proTree,tcp_ip_protocol_family::DissectResultIpv4 *dissectResultIpv4,void *reserves)
{
    Q_UNUSED(reserves)
    quint64 options = DissectResultBase::GetDissectorOptionPtr()->value("ipv4");

    // Summery
    qint32 start = dissectResultIpv4->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("ipv4");
    proTree->AddItem("ipv4"
                     ,QString("Internet Protocol Version %1, Src: %2, Dst: %3")
                     .arg(dissectResultIpv4->GetVersion())
                     .arg(dissectResultIpv4->GetSourceAddressStr())
                     .arg(dissectResultIpv4->GetDestinationAddressStr())
                     ,start
                     ,dissectResultIpv4->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("ipv4") - 1
                     ,ProTree::level::CURRENT
                     );

    // Summery - Version
    proTree->AddNextLayerItem("ipv4"
                     ,QString("%1 = Version: %2")
                     .arg(dissectResultIpv4->GetVersionDotStr())
                     .arg(dissectResultIpv4->GetVersion())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_VERSION_HDRLEN
                     ,false
                     );

    // Summery - Header Length
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("%1 = Header Length: %2 bytes (%3)")
                                 .arg(dissectResultIpv4->GetHeaderLengthDotStr())
                                 .arg(dissectResultIpv4->GetHeaderLength() * 4)
                                 .arg(dissectResultIpv4->GetHeaderLength())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_VERSION_HDRLEN
                                 ,true
                                 );

    // Summery - Differentiated Services Field
    proTree->AddCurrentLayerItem("ipv4"
                     ,QString("Differentiated Services field: %1 (DSCP: %2, ECN: %3)")
                     .arg(dissectResultIpv4->GetDifferentiatedServiceStr())
                     .arg(dissectResultIpv4->GetDSField_DSCP_short_meaning())
                     .arg(dissectResultIpv4->GetDSFField_ECN_short_meaning())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_DS
                     ,false);

    // Summery - Differentiated Services Field - DSCP
    proTree->AddNextLayerItem("ipv4"
                              ,QString("%1 = Differentiated Services Codepoint: %2 (%3)")
                              .arg(dissectResultIpv4->GetDSField_DSCP_DotStr())
                              .arg(dissectResultIpv4->GetDSField_DSCP_meanning())
                              .arg(dissectResultIpv4->GetDSField_DSCP())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_DS
                              ,false);


    // Summery - Differentiated Services Field - ECN
    proTree->AddCurrentLayerItem("ipv4"
                              ,QString("%1 = Explicit Congestion Notification: %2 (%3)")
                              .arg(dissectResultIpv4->GetDSField_ECN_DotStr())
                              .arg(dissectResultIpv4->GetDSField_ECN_meanning())
                              .arg(dissectResultIpv4->GetDSField_ECN())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_DS
                              ,true);

    // Summery - Total Length
    proTree->Pop();
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Total Length: %1")
                                 .arg(dissectResultIpv4->GetTotalLength())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_TOTALLEN
                                 ,true);

    // Summery - Identification
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Identification: %1 (%2)")
                                 .arg(dissectResultIpv4->GetIdentificationStr())
                                 .arg(dissectResultIpv4->GetIdentification())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_IDENT
                                 ,true);

    // Summery - Flags
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Flags: %1, %2")
                                 .arg(dissectResultIpv4->GetFlagOffsetStr())
                                 .arg(dissectResultIpv4->DF() ? "Don't fragment" : "More fragment")
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET
                                 ,false);

    // Summery - Flags - Reserved Bit
    proTree->AddNextLayerItem("ipv4"
                              ,QString("%1 = Reserved bit: %2")
                              .arg(dissectResultIpv4->GetReserveDotStr())
                              .arg(dissectResultIpv4->Reserve_meaning())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET
                              ,false);

    // Summery - Flags - DF
    proTree->AddCurrentLayerItem("ipv4"
                              ,QString("%1 = Don't fragment: %2")
                              .arg(dissectResultIpv4->GetDFDotStr())
                              .arg(dissectResultIpv4->DF_meaning())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET
                              ,false);

    // Summery - Flags - DF
    proTree->AddCurrentLayerItem("ipv4"
                              ,QString("%1 = More fragment: %2")
                              .arg(dissectResultIpv4->GetMFDotStr())
                              .arg(dissectResultIpv4->MF_meaning())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET
                              ,false);

    // Summery - Flags - Fragment Offset
    proTree->AddCurrentLayerItem("ipv4"
                              ,QString("%1 = Fragment offset: %2")
                              .arg(dissectResultIpv4->GetFragmentOffsetDotStr())
                              .arg(dissectResultIpv4->GetFragmentOffset())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET
                              ,true);

    // Summery - Time To Live
    proTree->Pop();
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Time to live: %1")
                                 .arg(dissectResultIpv4->GetTimeToLive())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_TTL
                                 ,true);

    // Summery - Protocol
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Protocol: %1 (%2)")
                                 .arg(dissectResultIpv4->GetDissectResultBase()->GetNextProtocolName("ipv4"))
                                 .arg(dissectResultIpv4->GetType())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_TYPE
                                 ,true);


    // Summery - Checksum
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Header checksum: %1")
                                 .arg(dissectResultIpv4->GetChecksumStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_HEADERCHECKSUM
                                 ,true);

    if( (options & IPV4_VALIDATE_CHECKSUM) && reserves != nullptr){

        bool equal =  dissectResultIpv4->GetChecksumStr() == dissectResultIpv4->GetCalculatedChecksumStr();
        // Summery - Checksum Status
        proTree->AddItem("ipv4"
                         ,QString("[Calculated checksum status: %1]")
                         .arg(equal ? "Good" : "Bad")
                         ,ProTree::level::CURRENT);

        // Summery - Calculated Checksum
        proTree->AddItem("ipv4"
                         ,QString("[Calculated checksum: %1]")
                         .arg(dissectResultIpv4->GetCalculatedChecksumStr())
                         ,ProTree::level::CURRENT);

    }

    // Summery - Source
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Source: %1")
                                 .arg(dissectResultIpv4->GetSourceAddressStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR
                                 ,true);

    // Summery - Destination
    proTree->AddCurrentLayerItem("ipv4"
                                 ,QString("Destination: %1")
                                 .arg(dissectResultIpv4->GetDestinationAddressStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_FIELD_LENGTH_DSTADDR
                                 ,true);

    // Summery
    proTree->Pop();
}
