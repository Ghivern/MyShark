#include "tcp.h"

Tcp::Tcp(ProTree *proTree,tcp_ip_protocol_family::DissectResultTcp *dissectResultTcp,void *reserves)
{
    Q_UNUSED(proTree)
    Q_UNUSED(dissectResultTcp)
    Q_UNUSED(reserves)

    proTree->AddItem("tcp",QString("TCP"));
}
