#include "udp.h"

Udp::Udp(ProTree *proTree,tcp_ip_protocol_family::DissectResultUdp *dissectResultUdp,void *reserves)
{
    Q_UNUSED(dissectResultUdp)
    Q_UNUSED(reserves)
    proTree->AddItem("udp","Udp");
}
