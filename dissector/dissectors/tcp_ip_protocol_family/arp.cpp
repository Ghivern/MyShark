#include "arp.h"

Arp::Arp(ProTree *proTree,tcp_ip_protocol_family::DissectResultArp *dissectResultArp,void *reserves)
{
    Q_UNUSED(dissectResultArp)
    Q_UNUSED(reserves)
    proTree->AddItem("arp","Arp");
}
