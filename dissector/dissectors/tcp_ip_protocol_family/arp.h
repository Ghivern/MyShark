#ifndef ARP_H
#define ARP_H

#include "../../protree/protree.h"
#include "../../dissres/dissectresutltframe.h"

//#include "../../dissres/tcp_ip_protocol_family/dissectresultarp.h"

class Arp
{
public:
    Arp(ProTree *proTree,tcp_ip_protocol_family::DissectResultArp *dissectResultArp,void *reserves);
};

#endif // ARP_H
