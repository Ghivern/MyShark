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
    void SetMsg(QString msg);
    void SetHeadersLen(qint32 headersLen);

    void AddToProtocolStackWithSE(QString protocol,qint32 len);
    void ResetProtocolStackAndPosition(QString protocol,qint32 newLen);
    void AddHeadersLen(qint32 headerLen);

    const uchar* GetData();
    const pcap_pkthdr* GetPkthdr();
    qint64 GetNo();
    timeval GetTimeval();
    float GetTimeSinceFirstPacket();
    qint32 GetCapLen();
    qint32 GetCapLenBit();
    qint32 GetLen();
    qint32 GetLenBit();
    qint32 GetHeadersLen();
    QString GetMsg();
    QString GetTopProtocol();
    qint32 GetProtocolStackLen();
    QString GetProtocolByIndex(qint32 index);
    QList<QString>& GetProtocolStack();
    qint32 GetProStart(QString proName);
    qint32 GetProEnd(QString proName);
protected:
    void addToProtocolStack(QString protocol);
    void addProPosition(QString proName,qint32 start,qint32 end);
    qint32 getTopProEnd();
    qint32 getTopProStart();
    bool proExist(QString proName);

    typedef struct position_t{
        qint32 start;
        qint32 end;
    }position_t;

    typedef struct packet_t{
        uchar *data;
        pcap_pkthdr *pkthdr;
    }packet_t;

private:
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
