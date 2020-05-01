#ifndef DISSECTRESULTBASE_H
#define DISSECTRESULTBASE_H

#include <QList>
#include <QHash>

#include "pcap.h"
#include "../units/tcpinfo.h"



/*
 * 主要功能
 * 1.获取各层协议首部的起始地址
 * 2.保留原始包数据的指针
 * 3.对保留字段的存取操作(向解析器传参的重要方法)
 */
class DissectResultBase
{
public:
    DissectResultBase(const quint8 *data,const pcap_pkthdr *pkther, qint64 index);
    ~DissectResultBase();

    void UpdateProtocolHeaderLengthCount(qint32 headerLength);
    void PushToProtocolList(QString protocolName, qint32 protocolHeaderLength);
    void UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength);
    void SetSummery(QString summery);

    bool ContainProtocol(QString proName);
    const quint8* GetData();
    const pcap_pkthdr* GetPkthdr();
    quint64 GetIndex();
    qint32 GetProtocolHeaderLengthCount();
    qint32 GetProtocolListLength();
    QString GetTopProtocolName();
    QString GetProtocolNameByIndex(qint32 index);
    QString GetProtocolsInFrame();
    const quint8* GetProtocolHeaderStartPtrByName(QString protocolName);
    qint64 GetProtocolHeaderStartPositionByName(QString protocolName);
    const quint8* GetProtocolHeaderEndPtrByName(QString protocolName);
    qint64 GetProtocolHeaderEndPositionByName(QString protocolName);
    QString GetSummery();


    /*保留字段的操作*/
    void ClearReserve();
    void ClearReserveVal();
    void ClearReservePtr();
    void AddAdditional(QString name,qint64 val);
    void AddAdditional(QString name,void *ptr,qint32 dataLen = 1);
    void AddAdditional(QString name,qint64 val,void *ptr,qint32 dataLen = 1);
    void OrToAddition(QString name,qint64 val);
    void RemoveAdditional(QString name);
    void RemoveAdditionalVal(QString name);
    void RemoveAdditionalPtr(QString name);
    qint64 GetAdditionalVal(QString name);
    void* GetAdditionalPtr(QString name,qint32 *len = nullptr);

    bool ReserveTcpContain(QString name);
    void AddAdditional(QString name,TcpInfo &tcpInfo);
    TcpInfo& GetAdditional(QString name);



private:
    typedef struct position_t{
        qint32 start;
        qint32 end;
    }position_t;

    /*保留字段，用于底层解析结果向高层解析结果传送数据*/
    struct reserve{
        void *ptr;
        qint32 len;
    };

    const quint8 *data;
    const pcap_pkthdr *pkthdr;
    qint64 index;
    QString summery;
    qint32 protocolHeaderLengthCount;
    QList<QString> protocolList;
    QHash<QString,position_t> protocolPositionHash;

    /*
     * 1.对保留字段的添加操作必须在AddNextLayer之前进行
     * 2.对保留字段的取操作，只需要保证在被清理之前使用就可以了
     * 3.不需要的数据要及时清理，不要轻易使用ClearReserveXX()方法，以免清理掉意想不到的数据
     */
    QHash<QString,struct reserve> reserve_ptr;
    QHash<QString,qint64> reserve_val;
    QHash<QString,TcpInfo> reserve_tcp;
};

#endif // DISSECTRESULTBASE_H
