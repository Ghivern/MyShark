#ifndef DISSECTRESUTLTFRAME_H
#define DISSECTRESUTLTFRAME_H
#include <QtCore>
#include <QList>
#include "time.h"
#include <arpa/inet.h>
#include "pcap.h"

#include "./dissectresultbase.h"
#include "./tcp_ip_protocol_family/dissectresultlinklayer.h"

class DissectResultFrame
{
public:
    typedef enum PROTOCOL_FAMILY_TYPE{
        TCP_IP_PROTOCOL_FAMILY = 1
    }PROTOCOL_FAMILY_TYPE;

    DissectResultFrame(const quint8 *data, const pcap_pkthdr *pkthdr, qint64 index, PROTOCOL_FAMILY_TYPE protocol_family_type = PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY);
    ~DissectResultFrame();

    void SetSummery(QString summery);
    void UpdateProtocolHeaderLengthCount(qint32 headerLength);
    void PushToProtocolList(QString protocolName, qint32 protocolHeaderLength, bool hide = false);
    void UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength);


    const quint8* GetData();
    qint64 GetIndex();
    quint64 GetCaptureTimeSec();
    quint64 GetCaptureTimeUSec();
    double GetRelativeTimeSinceFirstPacket();
    qint32 GetCapLen();
    qint32 GetLen();
    qint32 GetProtocolHeaderLengthCount();

    void* GetProtocolFamilyBaseLayer();
    /*定制化方法，获得特定类型的指针*/
    DissectResultFrame* GetTcpIpProtocolFamilyBaseLayer();

    QString GetSummery();
    qint32 GetProtocolListLength();
    QString GetProtocolNameByIndex(qint32 index);
    const quint8* GetProtocolHeaderStartPtrByName(QString protocolName);
private:
    typedef struct pcaket_t{
        quint8 *data;
        pcap_pkthdr *pkthdr;
    }packet_t;


    static bool isFirstPacket;
    static timeval firstPacketCaptureTime;

    packet_t packet;

    PROTOCOL_FAMILY_TYPE protocol_family_type;

    QString summery;
    void *protocol_family_base_layer;
    DissectResultBase *dissectResultBase;
};

#endif // DISSECTRESUTLTFRAME_H
