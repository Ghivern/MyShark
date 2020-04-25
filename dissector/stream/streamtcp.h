#ifndef STREAMTCP_H
#define STREAMTCP_H

#include <QStack>
#include <QtMath>

#include "stream.h"
#include "../units/keys.h"


#define TCP_A_RETRANSMISSION  0x0001
#define TCP_A_LOST_PACKET  0x0002
#define TCP_A_ACK_LOST_PACKET 0x0004
#define TCP_A_KEEP_ALIVE  0x0008
#define TCP_A_DUPLICATE_ACK  0x0010
#define TCP_A_ZERO_WINDOW  0x0020
#define TCP_A_ZERO_WINDOW_PROBE  0x0040
#define TCP_A_ZERO_WINDOW_PROBE_ACK  0x0080
#define TCP_A_KEEP_ALIVE_ACK  0x0100
#define TCP_A_OUT_OF_ORDER  0x0200
#define TCP_A_FAST_RETRANSMISSION  0x0400
#define TCP_A_WINDOW_SCALE_UPDATE  0x0800
#define TCP_A_WINDOW_FULL  0x1000
#define TCP_A_REUSED_PORTS  0x2000
#define TCP_A_SPURIOUS_RETRANSMISSION  0x4000
#define TCP_A_PREVIOUS_SEGMENT_NOT_CAPTURED 0x8000


const QHash<qint32,QString> tcp_segment_status_vals =
{
    {TCP_A_RETRANSMISSION,              "[Retransmission]"},
    {TCP_A_LOST_PACKET,                 "[Lost Packet]"},
    {TCP_A_ACK_LOST_PACKET,             "[Ack Lost Packet]"},
    {TCP_A_KEEP_ALIVE,                  "[Keep Alive]"},
    {TCP_A_DUPLICATE_ACK,               "[Duplicate Ack]"},
    {TCP_A_ZERO_WINDOW,                 "[Zero Window]"},
    {TCP_A_ZERO_WINDOW_PROBE,           "[Zero Window Probe]"},
    {TCP_A_ZERO_WINDOW_PROBE_ACK,       "[Zero Window Probe Ack]"},
    {TCP_A_KEEP_ALIVE_ACK,              "[Keep Alive Ack]"},
    {TCP_A_OUT_OF_ORDER,                "[Out Of Order]"},
    {TCP_A_FAST_RETRANSMISSION,         "[Fase Retransmission]"},
    {TCP_A_WINDOW_SCALE_UPDATE,         "[Window Scale Update]"},
    {TCP_A_WINDOW_FULL,                 "[Window Full]"},
    {TCP_A_REUSED_PORTS,                "[Reused Ports]"},
    {TCP_A_SPURIOUS_RETRANSMISSION,     "[Spurious Restrasmission]"},
    {TCP_A_PREVIOUS_SEGMENT_NOT_CAPTURED, "[Tcp Previous Segment Not Captured]"}
};

class StreamTcp:public Stream
{
public:

//    enum TCP_SEGMENT_STATUS{
//        TCP_A_RETRANSMISSION = 0x0001,
//        TCP_A_LOST_PACKET  =0x0002,
//        TCP_A_ACK_LOST_PACKET =0x0004,
//        TCP_A_KEEP_ALIVE = 0x0008,
//        TCP_A_DUPLICATE_ACK = 0x0010,
//        TCP_A_ZERO_WINDOW = 0x0020,
//        TCP_A_ZERO_WINDOW_PROBE = 0x0040,
//        TCP_A_ZERO_WINDOW_PROBE_ACK = 0x0080,
//        TCP_A_KEEP_ALIVE_ACK = 0x0100,
//        TCP_A_OUT_OF_ORDER = 0x0200,
//        TCP_A_FAST_RETRANSMISSION = 0x0400,
//        TCP_A_WINDOW_UPDATE = 0x0800,
//        TCP_A_WINDOW_FULL = 0x1000,
//        TCP_A_REUSED_PORTS = 0x2000,
//        TCP_A_SPURIOUS_RETRANSMISSION = 0x4000
//    };



    StreamTcp();
    qint64 AddWithWindow(DissectResultBase *dissectResultBase,quint8 *srcAddr,quint8 *dstAddr,qint32 addr_size,quint8 *srcPort,quint8 *dstPort,qint32 port_size = 2);
    quint32 GetBaseSeq(qint64 streamIndexPlusOne);
    quint16 GetWindowMultiplier(qint64 streamIndexPlusOne);

private:
    bool dealBaseSeq(DissectResultBase *dissectResultBase,qint64 streamIndexPlusOne);
    void addSegmentToWindow(DissectResultBase *dissectResultBase,qint64 streamIndexPlusOne);
    qint32 addSegmentFromOutOfOrderListToWindow(DissectResultBase *dissectResultBase,qint64 streamIndexPlusOne);
    void addSegmentToOutOfOrderList(DissectResultBase *dissectResultBase,qint64 streamIndexPlusOne);
    quint32 getSeriesSegmentMaxCalculateSeq(quint32 seq,quint32 payloadLen,qint64 streamIndexPlusOne);

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
        QHash<quint16,QList<quint64>> acks;
    };

    /*streamIndexPlusOne --> window*/
    QHash<qint64,struct window_t> windows;

    /*ack --> packet List*/
    //QHash<quint16,QList<quint64>> acks;
};

#endif // STREAMTCP_H
