#ifndef DISSRESETH_H
#define DISSRESETH_H
#include "dissres.h"

class DissResEth:public DissRes
{
public:
    DissResEth(qint64 no);
    void SetIpv6();
    void SetMacSrc(uchar *macSrc);
    void SetMacDst(uchar *macDst);
    void SetTv(timeval tv);
    void SetDisplayTv(timeval tv);
    void SetIpSrc(uchar *ipSrc);
    void SetIpDst(uchar *ipDst);
    void SetIpv4Src(uchar *ipSrc);
    void SetIpv4Dst(uchar *ipDst);
    void SetCapLen(qint32 capLen);
    void AddToProtocolStack(QString protocol);
    void AddToProtocolStackWithSE(QString protocol,qint32 len);
    void SetHeadersLen(qint32 headersLen);
    void AddHeadersLen(qint32 headerLen);
    void AddProPosition(QString proName,qint32 start,qint32 end);
    void SetSrcPort(ushort srcPort);
    void SetDstPort(ushort dstPort);
    void SetMsg(QString msg);

    qint64 GetNo();
    QString GetStrMacSrc();
    QString GetStrMacDst();
    float GetTimeSinceFirstPacket();
    QString GetStrIpSrc();
    QString GetStrIpDst();
    qint32 GetCapLen();
    QString GetTopProtocol();
    QList<QString>& GetProtocolStack();
    qint32 GetHeadersLen();
    qint32 GetProStart(QString proName);
    qint32 GetProEnd(QString proName);
    qint32 GetTopProEnd();
    ushort GetSrcPort();
    ushort GetDstPort();
    QString GetMsg();

private:
    typedef enum fieldLen{
        MAC_ADDRESS = 6,
        IP_ADDRESS = 4,
        IPV6_ADDRESS = 16
    }fieldLen;

    typedef struct position_t{
        qint32 start;
        qint32 end;
    }position_t;

    static bool isFirstPacket;
    static timeval firstTv;
    bool isIpv6 = false;
    qint64 no;
    timeval tv;
    timeval displayTv;
    uchar macSrc[6] = {0};
    uchar macDst[6] = {0};
    uchar ipSrc[16] = {0};
    uchar ipDst[16] = {0};
    qint32 capLen;
    QList<QString> protocolStack;
    QHash<QString,position_t> positionStack;
    qint32 headersLen;
    ushort srcPort = 0;
    ushort dstPort = 0;
    QString msg;
};

#endif // DISSRESETH_H
