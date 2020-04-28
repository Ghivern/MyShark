#ifndef TCPIPPROTOCOLFAMILY_H
#define TCPIPPROTOCOLFAMILY_H

#include "../protree/protree.h"
#include "../dissres/dissectresutltframe.h"

#include "frame.h"
#include "frame.h"
#include "./tcp_ip_protocol_family/linklayer.h"
#include "./tcp_ip_protocol_family/ipv4.h"
#include "./tcp_ip_protocol_family/arp.h"
#include "./tcp_ip_protocol_family/tcp.h"
#include "./tcp_ip_protocol_family/udp.h"
#include "http.h"


class TcpIpProtocolFamily
{
public:
    TcpIpProtocolFamily(ProTree *proTree,DissectResultFrame *dissectResultFrame,void *reserves);

private:
    void dealIpv4Type(ProTree *proTree,tcp_ip_protocol_family::DissectResultIpv4 *ipv4,void *reserves);
    void dealTcpType(ProTree *proTree,tcp_ip_protocol_family::DissectResultTcp *tcp,void *reserves);
    void dealUdpType(ProTree *proTree,tcp_ip_protocol_family::DissectResultUdp *udp,void *reserves);
};

#endif // TCPIPPROTOCOLFAMILY_H
