#ifndef DISSRES_H
#define DISSRES_H
#include <QtCore>
#include "time.h"
#include "pcap.h"
#include <arpa/inet.h>
#include <QList>

class DissRes{
public:
    DissRes(qint64 no);
    ~DissRes();
    void SetPacket(const uchar *data,const pcap_pkthdr *pkthdr);
    void AddToProtocolStack(QString protocol);
    void AddToProtocolStackWithSE(QString protocol,qint32 len);
    void SetHeadersLen(qint32 headersLen);
    void AddHeadersLen(qint32 headerLen);
    void AddProPosition(QString proName,qint32 start,qint32 end);
    void SetMsg(QString msg);

    const uchar* GetData();
    const pcap_pkthdr* GetPkthdr();
    qint64 GetNo();
    timeval GetTimeval();
    float GetTimeSinceFirstPacket();
    qint32 GetCapLen();
    qint32 GetCapLenBit();
    qint32 GetLen();
    qint32 GetLenBit();
    QString GetTopProtocol();
    QString GetProtocolByIndex(qint32 index);
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

    typedef struct packet_t{
        uchar *data;
        pcap_pkthdr *pkthdr;
    }packet_t;

    static bool isFirstPacket;
    static timeval firstTv;

    packet_t packet;
    qint64 no;
    QList<QString> protocolStack;
    QHash<QString,position_t> positionStack;
    qint32 headersLen = 0;
    QString msg;
};

typedef QList<DissRes*> DissResList;

#endif // DISSRES_H
