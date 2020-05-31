#ifndef UDPINFP_H
#define UDPINFP_H

#include <QtCore>

class UdpInfo
{
public:
    UdpInfo();

    qint32 totalLen;

    bool badChecksum;
};

#endif // UDPINFP_H
