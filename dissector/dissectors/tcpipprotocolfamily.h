#ifndef TCPIPPROTOCOLFAMILY_H
#define TCPIPPROTOCOLFAMILY_H

#include "../protree/protree.h"
#include "../dissres/dissectresutltframe.h"

#include "frame.h"
#include "frame.h"
#include "./tcp_ip_protocol_family/linklayer.h"
#include "./tcp_ip_protocol_family/ipv4.h"

class TcpIpProtocolFamily
{
public:
    TcpIpProtocolFamily(ProTree *proTree,DissectResultFrame *dissectResultFrame,void *reserves);
};

#endif // TCPIPPROTOCOLFAMILY_H
