#ifndef STREAMINDEX_H
#define STREAMINDEX_H
#include <QHash>
#include <QList>
#include <QString>
#include <QCryptographicHash>

class StreamIndex
{
public:
    StreamIndex();
    qint64 Add(QString addressS,QString addressD,quint16 portS,quint16 portD,qint64 streamItem_index);
    qint64 GetStreamItemIndex(QString addressS,QString addressD,quint16 portS,quint16 portD);

private:
    QHash<QByteArray,qint64> streamIndex_hash;

    qint32 exist(QString addressS,QString addressD,quint16 portS,quint16 portD,qint64 *streamIndex);
};

#endif // STREAMINDEX_H
