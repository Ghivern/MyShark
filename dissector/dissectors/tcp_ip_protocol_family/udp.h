#ifndef UDP_H
#define UDP_H

#include "../../protree/protree.h"
#include "../../dissres/dissectresutltframe.h"

class Udp
{
public:
    Udp(ProTree *proTree,tcp_ip_protocol_family::DissectResultUdp *dissectResultUdp,void *reserves);
};

#endif // UDP_H
