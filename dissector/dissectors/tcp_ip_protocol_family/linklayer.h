#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "../../protree/protree.h"
#include "../../dissres/dissectresutltframe.h"


class Linklayer
{
public:
    Linklayer(ProTree *proTree,tcp_ip_protocol_family::DissectResultLinkLayer *dissectResultLinklayer,void *reserves);
};

#endif // LINKLAYER_H
