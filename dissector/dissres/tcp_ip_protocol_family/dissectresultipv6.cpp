#include "dissectresultipv6.h"

using namespace tcp_ip_protocol_family;

DissectResultIpv6::DissectResultIpv6(DissectResultBase *dissectResultBase,void *reserves)
            :DissectResult(dissectResultBase)
{
    Q_UNUSED(reserves)
    Q_UNUSED(dissectResultBase)
}
