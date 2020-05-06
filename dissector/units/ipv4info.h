#ifndef IPV4INFO_H
#define IPV4INFO_H

#include <QtCore>

class Ipv4Info
{
public:
    Ipv4Info();
    quint8 *header;
    qint32 headerLen;

    qint32 payloadLen;

    quint8 *src;
    quint8* dst;
    qint32 addrLen;

    quint64 status = 0;

};

#endif // IPV4INFO_H
