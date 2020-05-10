#ifndef STREAMTCP2_H
#define STREAMTCP2_H

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
#define TCP_A_WINDOW_UPDATE  0x0800
#define TCP_A_WINDOW_FULL  0x1000
#define TCP_A_REUSED_PORTS  0x2000
#define TCP_A_SPURIOUS_RETRANSMISSION  0x4000

#define TCP_S_SAW_SYN 0x010000
#define TCP_S_SAW_SYNACK 0x020000
#define TCP_S_BASE_SEQ_SET 0x040000

const QHash<qint32,QString> tcp_segment_status_vals =
{
    {TCP_A_RETRANSMISSION,              "[Retransmission]"},
    {TCP_A_LOST_PACKET,                 "[Previous segment not captured]"},
    {TCP_A_ACK_LOST_PACKET,             "[ACKed unseen segment]"},
    {TCP_A_KEEP_ALIVE,                  "[Keep Alive]"},
    {TCP_A_DUPLICATE_ACK,               "[Duplicate Ack %1#%2]"},
    {TCP_A_ZERO_WINDOW,                 "[Zero Window]"},
    {TCP_A_ZERO_WINDOW_PROBE,           "[Zero Window Probe]"},
    {TCP_A_ZERO_WINDOW_PROBE_ACK,       "[Zero Window Probe Ack]"},
    {TCP_A_KEEP_ALIVE_ACK,              "[Keep Alive Ack]"},
    {TCP_A_OUT_OF_ORDER,                "[Out Of Order]"},
    {TCP_A_FAST_RETRANSMISSION,         "[Fase Retransmission]"},
    {TCP_A_WINDOW_UPDATE,               "[Window Update]"},
    {TCP_A_WINDOW_FULL,                 "[Window Full]"},
    {TCP_A_REUSED_PORTS,                "[Reused Ports]"},
    {TCP_A_SPURIOUS_RETRANSMISSION,     "[Spurious Restrasmission]"},
};


class StreamTcp2:public Stream
{
public:
     StreamTcp2();

     qint64 AddWithWindow(DissectResultBase *dissectResultBase,quint8 *srcAddr,quint8 *dstAddr,qint32 addr_size,quint8 *srcPort,quint8 *dstPort,qint32 port_size = 2);

     void ClearWindow(qint64 streamIndexPlusOne);

     quint32 GetBaseSeq(qint64 streamIndexPlusOne);
     quint32 GetWindowMultiplier(qint64 streamIndexPlusOne);

     void Clear();

     class Window{
     public:
         Window(){
            this->windowVal = 0;
            this->windowScale = -1;

            this->flags = 0;

            this->baseSeq = 0;
            this->nextSeq = 0;
            this->maxSeqToBeAcked = 0;

            this->lastAck = 0;
            this->lastAckTime.tv_sec = 0;
            this->lastAckTime.tv_usec = 0;

            this->first_rtt.tv_sec = 0;
            this->first_rtt.tv_usec = 0;
            this->haveFirstRrt = false;

            this->lastSegmentFlags = 0;

            this->lastNonDupAck = 0;
            this->dupAckNum = 0;
         }

         qint32 windowVal;
         qint32 windowScale;

         quint32 flags;

         quint32 baseSeq;
         quint32 nextSeq;
         timeval nextSeqTime;
         quint32 maxSeqToBeAcked;

         quint32 lastAck;
         timeval lastAckTime;

         timeval first_rtt;
         bool haveFirstRrt;

         quint32 lastSegmentFlags;

         qint64 lastNonDupAck;
         qint32 dupAckNum;
     };

private:

     QHash<qint64,Window> allWindow;
};

#endif // STREAMTCP2_H
