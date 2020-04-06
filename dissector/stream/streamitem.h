#ifndef STREAMITEM_H
#define STREAMITEM_H
#include <QList>
#include <QHash>
#include <QString>

class StreamItem
{
public:
    class Position_Direction{
    public:
        Position_Direction(qint64 dissRes_index);
        bool Modify(qint64 dissRes_index,bool direction);
    private:
        QHash<bool,QList<qint64>*> dissRes_index_hash;
    };

    StreamItem(QString addressA,QString addressB,quint32 portA,quint32 portB,qint64 dissRes_index,qint64 *streamIndex);
    ~StreamItem();
    void Modify(qint64 dissRes_index, QString addressS,qint64 *streamIndex);

private:
    QString addressA;
    QString addressB;
    quint16 portA;
    quint16 portB;
    qint64 streamIndexAB;
    qint64 streamIndexBA;
    Position_Direction *pd;
};

#endif // STREAMITEM_H
