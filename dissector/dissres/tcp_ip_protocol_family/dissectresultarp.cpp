#include "dissectresultarp.h"

using namespace tcp_ip_protocol_family;

DissectResultArp::DissectResultArp(DissectResultBase *dissectResultBase,void *reserves)
        :DissectResult(dissectResultBase)
{
    Q_UNUSED(reserves)
    Q_UNUSED(dissectResultBase)
    dissectResultBase->PushToProtocolList("arp",0);
}
