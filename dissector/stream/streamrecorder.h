#ifndef STREAMRECORDER_H
#define STREAMRECORDER_H
#include "streamindex.h"
#include "streamitem.h"
#include <QList>

class StreamRecorder
{
public:
    StreamRecorder();
    void Add(QString addressS,QString addressD,quint16 portS,quint16 portD,qint64 dissRes_index);
    void Add(QString addressS,QString addressD,qint64 dissRes_index);
    qint64 GetStreamIndex(QString addressS,QString addressD,quint16 portS,quint16 portD);
    qint64 GetStreamItemCount();
    StreamItem* GetStreamItemByIndex(qint64 index);

private:
    qint64 streamIndex = 0;
    QList<StreamItem*> streams;
    StreamIndex index;

};

#endif // STREAMRECORDER_H
