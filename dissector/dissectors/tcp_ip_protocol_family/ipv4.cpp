#include "ipv4.h"

Ipv4::Ipv4(ProTree *proTree,tcp_ip_protocol_family::DissectResultIpv4 *dissectResultIpv4,void *reserves)
{
    Q_UNUSED(reserves)

    // Summery
    qint32 start = dissectResultIpv4->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("ipv4");
    proTree->AddItem("ipv4",QString("Internet Protocol Version %1, Src: %2, Dst: %3")
                     .arg(dissectResultIpv4->GetVersion())
                     .arg(dissectResultIpv4->GetSourceAddressStr())
                     .arg(dissectResultIpv4->GetDestinationAddressStr())
                     ,start
                     ,dissectResultIpv4->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("ipv4") - 1
                     ,ProTree::level::CURRENT
                     );
}
