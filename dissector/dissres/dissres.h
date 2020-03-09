#ifndef DISSRES_H
#define DISSRES_H
#include <QtCore>
#include "time.h"
#include <arpa/inet.h>
#include <QList>

class DissRes{
public:
    DissRes(qint64 no);
    void SetTv(timeval tv);
    void SetCapLen(qint32 capLen);
    void AddToProtocolStack(QString protocol);
    void AddToProtocolStackWithSE(QString protocol,qint32 len);
    void SetHeadersLen(qint32 headersLen);
    void AddHeadersLen(qint32 headerLen);
    void AddProPosition(QString proName,qint32 start,qint32 end);
    void SetMsg(QString msg);

    qint64 GetNo();
    float GetTimeSinceFirstPacket();
    qint32 GetCapLen();
    QString GetTopProtocol();
    QList<QString>& GetProtocolStack();
    qint32 GetHeadersLen();
    qint32 GetProStart(QString proName);
    qint32 GetProEnd(QString proName);
    qint32 GetTopProEnd();
    QString GetMsg();
protected:
    typedef struct position_t{
        qint32 start;
        qint32 end;
    }position_t;

    static bool isFirstPacket;
    static timeval firstTv;

    qint64 no;
    timeval tv;
    qint32 capLen;
    QList<QString> protocolStack;
    QHash<QString,position_t> positionStack;
    qint32 headersLen;
    QString msg;
};

#endif // DISSRES_H
