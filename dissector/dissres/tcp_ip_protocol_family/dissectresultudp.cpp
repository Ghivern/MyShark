#include "dissectresultudp.h"

using namespace tcp_ip_protocol_family;

DissectResultUdp::DissectResultUdp(DissectResultBase *dissectResultBase)
{
    Q_UNUSED(dissectResultBase)
    dissectResultBase->PushToProtocolList("udp",0);
}
