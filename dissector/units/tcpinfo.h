#ifndef TCPINFO_H
#define TCPINFO_H

#include <QtCore>

class TcpInfo
{
public:
    TcpInfo();

    quint32 seq;
    quint32 ack;
    quint32 segLen;
    quint32 totalLen;

    bool ACK;
    bool SYN;
    bool RST;
    bool FIN;

    qint32 windowSclae;
    qint32 windowVal;

    timeval time;
    timeval echoReplayTime;

    qint32 status;
    bool badChecksum;

    qint32 dupack_frame;
    qint32 dupack_num;

    qint32 streamPlusOne;

};

#endif // TCPINFO_H
