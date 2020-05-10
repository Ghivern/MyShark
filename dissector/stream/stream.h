#ifndef STREAM_H
#define STREAM_H

#include <QCryptographicHash>
#include <QHash>
#include <QList>
#include <QByteArray>

#include "arpa/inet.h"

#include "../dissres/dissectresultbase.h"

class Stream
{
public:
    Stream(qint32 addrSize = 4,qint32 portSize = 0);
    /*
     * 返回streamIndex(未取绝对值并减1),便于子类TcpStream拓展
     */
    qint64 Add(DissectResultBase *dissectResultBase,quint8 *srcAddr,quint8 *dstAddr,qint32 addr_size,quint8 *srcPort = nullptr,quint8 *dstPort = nullptr,qint32 port_size = 0);
    qint64 Add(DissectResultBase *dissectResultBase,quint8 *srcAddr,quint8 *dstAddr,quint8 *srcPort = nullptr,quint8 *dstPort = nullptr);

    /*统计Conversion时使用*/
    qint32 GetStreamCount();  /*正数流的个数*/
    bool Contain(qint64 streamIndexPlusOne);
    /*若流不存在，则返回一个空流*/
    QList<quint64> GetPacketsIndexListByStream(qint64 streamIndexPlusOne);
    /*以下四个方法，只用于获取正向流的信息*/
//    QString GetSourceAddress(qint64 streamIndexPlusOne);
    quint16 GetSourcePort(qint64 streamIndexPlusOne);
//    QString GetDestinationAddress(qint64 streamIndexPlusOne);
    quint16 GetDestinationPort(qint64 streamIndexPlusOne);

    bool HavePort();

    void ClearStream();

protected:

    qint32 addrSize;
    qint32 portSize;


private:
    struct indexes_streamIndex{
      QList<quint64> indexes;
       /*从1开始记数，避免0没有负值的情况，符号用于区分流的方向*/
      qint64 streamIndexPlusOne;
//      QString srcAddr;
      quint16 srcPort;
//      QString dstAddr;
      quint16 dstPort;
    };

    qint64 maxStreamIndexPlusOne;
    QHash<QByteArray,struct indexes_streamIndex> streamHash;

    /*用于统计,通过流号找到地址的Md5,进而找到indexes_streamIndex结构体*/
    QHash<qint64,QByteArray> md5Hash;
};

#endif // STREAM_H
