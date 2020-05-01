#include "dissectresultudp.h"

using namespace tcp_ip_protocol_family;

DissectResultUdp::DissectResultUdp(DissectResultBase *dissectResultBase,void *reserves):DissectResult(dissectResultBase)
{
    Q_UNUSED(reserves)
    Q_UNUSED(dissectResultBase)
    dissectResultBase->PushToProtocolList("udp",0);
}
