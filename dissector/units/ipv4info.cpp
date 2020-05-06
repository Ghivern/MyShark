#include "ipv4info.h"

Ipv4Info::Ipv4Info()
{
    this->header = nullptr;
    this->headerLen = 0;
    this->payloadLen = 0;

    this->src = nullptr;
    this->dst = nullptr;
    this->addrLen = 0;

    this->status = 0;
}
