#ifndef TUSTREAM_H
#define TUSTREAM_H
#include <QtCore>
#include <QList>
class TUStream
{
public:
    TUStream();
    qint64 Exist(QString srcAddress,ushort srcPort,QString dstAddress,ushort dstPort,bool& AB);
    void Append(QString srcAddress,ushort srcPort,QString dstAddress,ushort dstPort);
    qint64 StreamIndex(QString srcAddress,ushort srcPort,QString dstAddress,ushort dstPort);
    QString GetAddressA(qint64 index);
    QString GetAddressB(qint64 index);
    ushort GetPortA(qint64 index);
    ushort GetPortB(qint64 index);
private:
    typedef struct connect_t{
        QString addressA;
        ushort portA;
        QString addressB;
        ushort portB;

    }connect_t;
    QList<connect_t> connects;
};

#endif // TUSTREAM_H
