#include "arp.h"

Arp::Arp(ProTree *proTree,tcp_ip_protocol_family::DissectResultArp *dissectResultArp,void *reserves)
{
    Q_UNUSED(dissectResultArp)
    Q_UNUSED(reserves)

    qint32 start = dissectResultArp->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("arp");

    // Summery
    proTree->AddItem("arp"
                     ,QString("Address Resolution Protocol ( %1 )")
                     .arg(dissectResultArp->GetOpereation())
                     ,start
                     ,dissectResultArp->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("arp") - 1
                     ,ProTree::level::CURRENT
                     );

//    enum ARP_FIELD_LENGTH{
//      HARDWARETYPE = 2,
//      PROTOCOLTYPE = 2,
//      MACADDRLEN = 1,
//      PROTOCOLADDRLEN = 1,
//      OPCODE = 2,
//      SRCMAC = 6,
//      SRCIP = 4,
//      DSTMAC= 6,
//      DSTIP = 4,

//      ARP_TEMP_TOTAL_LEN = 26
//    };

    //Summery - Hardware Type
    proTree->AddNextLayerItem("arp"
                              ,QString("Hardware type: %1 (%2)")
                              .arg(dissectResultArp->GetHardwareTypeStr())
                              .arg(dissectResultArp->GetHardwareType())
                              ,&start
                              ,tcp_ip_protocol_family::DissectResultArp::HARDWARETYPE
                              ,true);

    //Summery - Protocol Type
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Protocol type: %1 (%2)")
                                 .arg(dissectResultArp->GetProtocolTypeName())
                                 .arg(dissectResultArp->GetProtocolTypeStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::PROTOCOLTYPE
                                 ,true);

    //Summery - Hardware size
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Hardware size: %1")
                                 .arg(dissectResultArp->GetMacAddrLen())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::MACADDRLEN
                                 ,true);

    //Summery - Protocol size
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Protocol size: %1")
                                 .arg(dissectResultArp->GetProtocolAddrLen())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::PROTOCOLADDRLEN
                                 ,true);

    //Summery - Opcode
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Opcode: %1 (%2)")
                                 .arg(dissectResultArp->GetOpereation())
                                 .arg(dissectResultArp->GetOpCode())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::OPCODE
                                 ,true);

    //Summery - Sender MAC address
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Sender Mac addess: %1")
                                 .arg(dissectResultArp->GetSrcMacStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::SRCMAC
                                 ,true);

    //Summery - Sender Ip address
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Sender Ip address: %1")
                                 .arg(dissectResultArp->GetSrcIpStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::SRCIP
                                 ,true);

    //Summery - Target Mac address
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Target Mac addess: %1")
                                 .arg(dissectResultArp->GetDstMacStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::DSTMAC
                                 ,true);

    //Summery - Target Ip address
    proTree->AddCurrentLayerItem("arp"
                                 ,QString("Target Ip addess: %1")
                                 .arg(dissectResultArp->GetDstIpStr())
                                 ,&start
                                 ,tcp_ip_protocol_family::DissectResultArp::DSTIP
                                 ,true);


    proTree->Pop();
}
