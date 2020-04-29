#include "dissectresultarp.h"

using namespace tcp_ip_protocol_family;

DissectResultArp::DissectResultArp(DissectResultBase *dissectResultBase,void *reserves)
{
    Q_UNUSED(reserves)
    Q_UNUSED(dissectResultBase)
    dissectResultBase->PushToProtocolList("arp",0);
}
