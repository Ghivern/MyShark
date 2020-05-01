#ifndef STREAM_H
#define STREAM_H

#include <QCryptographicHash>
#include <QHash>
#include <QList>
#include <QByteArray>



#include "../dissres/dissectresultbase.h"

class Stream
{
public:
    Stream();
    /*
     * 返回streamIndex(未取绝对值并减1),便于子类TcpStream拓展
     */
    qint64 Add(DissectResultBase *dissectResultBase,quint8 *srcAddr,quint8 *dstAddr,qint32 addr_size,quint8 *srcPort = NULL,quint8 *dstPort = NULL,qint32 port_size = 2);

    /*统计Conversion时使用*/
    QList<quint64> GetPacketsIndexListByStream(qint64 streamIndexPlusOne);

protected:
    void ClearStream();


private:
    struct indexes_streamIndex{
      QList<quint64> indexes;
       /*从1开始记数，避免0没有负值的情况，符号用于区分流的方向*/
      qint64 streamIndexPlusOne;
    };

    qint64 maxStreamIndexPlusOne;
    QHash<QByteArray,struct indexes_streamIndex> streamHash;

    /*用于统计,通过流号找到地址的Md5,进而找到indexes_streamIndex结构体*/
    QHash<qint64,QByteArray> md5Hash;
};

#endif // STREAM_H
