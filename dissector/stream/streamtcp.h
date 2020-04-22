#ifndef STREAMTCP_H
#define STREAMTCP_H

#include <QStack>
#include <QtMath>

#include "stream.h"
#include "../units/keys.h"


class StreamTcp:public Stream
{
public:

    enum TCP_SEGMENT_STATUS{
        TCP_A_RETRANSMISSION = 0x0001,
        TCP_A_LOST_PACKET  =0x0002,
        TCP_A_ACK_LOST_PACKET =0x0004,
        TCP_A_KEEP_ALIVE = 0x0008,
        TCP_A_DUPLICATE_ACK = 0x0010,
        TCP_A_ZERO_WINDOW = 0x0020,
        TCP_A_ZERO_WINDOW_PROBE = 0x0040,
        TCP_A_ZERO_WINDOW_PROBE_ACK = 0x0080,
        TCP_A_KEEP_ALIVE_ACK = 0x0100,
        TCP_A_OUT_OF_ORDER = 0x0200,
        TCP_A_FAST_RETRANSMISSION = 0x0400,
        TCP_A_WINDOW_UPDATE = 0x0800,
        TCP_A_WINDOW_FULL = 0x1000,
        TCP_A_REUSED_PORTS = 0x2000,
        TCP_A_SPURIOUS_RETRANSMISSION = 0x4000
    };



    StreamTcp();
    qint64 AddWithWindow(DissectResultBase *dissectResultBase,quint8 *srcAddr,quint8 *dstAddr,qint32 addr_size,quint8 *srcPort,quint8 *dstPort,qint32 port_size = 2);
    quint32 GetBaseSeq(qint64 streamIndexPlusOne);
    quint16 GetWindowMultiplier(qint64 streamIndexPlusOne);

private:

    struct segment_t{
      quint32 seq;
      quint32 len;
      quint64 index;
    };

    struct window_t{
        QList<segment_t> segmentList;
        QList<segment_t> outOfOrderSegmentList;
        quint32 maxSeq;
        quint32 baseSeq;
        qint16 windowMultiplier;
    };

    /*streamIndexPlusOne --> window*/
    QHash<qint64,struct window_t> windows;

    /*ack --> packet List*/
    QHash<quint16,QList<quint64>> acks;
};

#endif // STREAMTCP_H
