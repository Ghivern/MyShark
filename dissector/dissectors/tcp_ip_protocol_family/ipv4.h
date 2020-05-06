#ifndef IPV4_H
#define IPV4_H

#include "../../protree/protree.h"
#include "../../dissres/dissectresutltframe.h"

#include "../../units/checksum2.h"

class Ipv4
{
public:
    Ipv4(ProTree *proTree,tcp_ip_protocol_family::DissectResultIpv4 *dissectResultIpv4,void *reserves);
};

#endif // IPV4_H
