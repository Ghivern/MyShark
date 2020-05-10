#ifndef DISSECTRESUTLTFRAME_H
#define DISSECTRESUTLTFRAME_H
#include <QtCore>
#include <QList>
#include "time.h"
#include <arpa/inet.h>
#include "pcap.h"

#include "./dissectresultbase.h"

#include "./dissectresult.h"

#include "./tcp_ip_protocol_family/dissectresultlinklayer.h"

class DissectResultFrame
{
public:


    typedef enum PROTOCOL_FAMILY_TYPE{
        TCP_IP_PROTOCOL_FAMILY = 1
    }PROTOCOL_FAMILY_TYPE;

    DissectResultFrame(const quint8 *data, const pcap_pkthdr *pkthdr, qint64 index
                       ,PROTOCOL_FAMILY_TYPE protocol_family_type = PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY
                       ,void *reserves = nullptr);
    ~DissectResultFrame();

//    void SetSummery(QString summery);
//    void UpdateProtocolHeaderLengthCount(qint32 headerLength);
//    void PushToProtocolList(QString protocolName, qint32 protocolHeaderLength);
//    void UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength);

//    const quint8* GetData();
//    quint64 GetIndex();
    qint64 GetCaptureTimeSec();
    qint64 GetCaptureTimeUSec();
    double GetRelativeTimeSinceFirstPacket();
    qint32 GetCapLen();
    qint32 GetLen();
//    qint32 GetProtocolHeaderLengthCount();

    DissectResultBase *GetDissectResultBase();
    void* GetProtocolFamilyBaseLayer();
    /*定制化方法，获得特定类型协议族的数据链路层解析结果指针*/
    tcp_ip_protocol_family::DissectResultLinkLayer* GetTcpIpProtocolFamilyBaseLayer();

    static void Clear();

//    QString GetSummery();
//    qint32 GetProtocolListLength();
//    QString GetProtocolNameByIndex(qint32 index);
//    const quint8* GetProtocolHeaderStartPtrByName(QString protocolName);

//    QString GetTopProtocolName();
//    bool ContainProtocol(QString proName);


private:
    void addFirstLayer(PROTOCOL_FAMILY_TYPE type,void *reserves);

    static bool isFirstPacket;
    static timeval firstPacketCaptureTime;
    static QList<DissectResultBase*> *dissectResultBaseList;

    pcap_pkthdr *pkthdr;

    /*构造函数中用于判断应该新建何种协议族的DissectResultLinkLayer*/
//    PROTOCOL_FAMILY_TYPE protocol_family_type;

    void *protocol_family_base_layer;
    DissectResultBase *dissectResultBase;
};

#endif // DISSECTRESUTLTFRAME_H
