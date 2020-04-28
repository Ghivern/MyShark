#ifndef TCP_H
#define TCP_H

#include "../../protree/protree.h"
#include "../../dissres/dissectresutltframe.h"

class Tcp
{
public:
    Tcp(ProTree *proTree,tcp_ip_protocol_family::DissectResultTcp *dissectResultTcp,void *reserves);
};

#endif // TCP_H
