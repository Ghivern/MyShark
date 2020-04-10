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

    void AddToProtocolStackWithSE(QString protocol,qint32 len,bool hide = false);
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
    const quint8* GetProStartPtr(QString proName);
    qint32 GetProEnd(QString proName);
    const quint8* GetProEndPtr(QString proName);



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
    static bool isFirstPacket;  //第一个包进入时其值为true，解析完成，置为false;
    static timeval firstTv; //结合isFirstPacket,记录第一个包的到达时间


    packet_t packet;  //存储指向原始数据的指针
    qint64 no;  //记录在自己在列表中的位置
    QList<QString> protocolStack;   //协议栈： eth:ipv4:tcp ,以此种结构存储包中的协议栈
    QHash<QString,position_t> positionStack;  //位置栈：存协议栈中各个协议段的起始位置和结束位置
    qint32 headersLen = 0;
    QString msg;  //包解析结果的简明总结
};

typedef QList<DissRes*> DissResList_t;

#endif // DISSRES_H
