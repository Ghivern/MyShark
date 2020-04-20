#ifndef DISSECTRESULTBASE_H
#define DISSECTRESULTBASE_H

#include <QList>
#include <QHash>

#include "pcap.h"



/*
 * 主要功能就是获取各层协议首部的起始地址
 */
class DissectResultBase
{
public:
    DissectResultBase(const quint8 *data,const pcap_pkthdr *pkther, qint64 index);
    ~DissectResultBase();

    void UpdateProtocolHeaderLengthCount(qint32 headerLength);
    void PushToProtocolList(QString protocolName, qint32 protocolHeaderLength);
    void UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength);
    bool ContainProtocol(QString proName);
    QString GetTopProtocolName();

    void SetSummery(QString summery);

    const quint8* GetData();
    const pcap_pkthdr* GetPkthdr();
    quint64 GetIndex();
    qint32 GetProtocolHeaderLengthCount();
    qint32 GetProtocolListLength();
    QString GetProtocolNameByIndex(qint32 index);
    const quint8* GetProtocolHeaderStartPtrByName(QString protocolName);
    const quint8* GetProtocolHeaderEndPtrByName(QString protocolName);
    QString GetSummery();

    /*保留字段的操作*/
    void ClearReserve();
    void ClearReserveVal();
    void ClearReservePtr();
    void AddAdditional(QString name,qint64 val);
    void AddAdditional(QString name,void *ptr,qint32 dataLen = 1);
    void AddAdditional(QString name,qint64 val,void *ptr,qint32 dataLen = 1);
    void RemoveAdditional(QString name);
    void RemoveAdditionalVal(QString name);
    void RemoveAdditionalPtr(QString name);
    qint64 GetAdditionalVal(QString name);
    void* GetAdditionalPtr(QString name,qint32 *len = NULL);



private:
    typedef struct position_t{
        qint32 start;
        qint32 end;
    }position_t;

    const quint8 *data;
    const pcap_pkthdr *pkthdr;
    qint64 index;
    qint32 protocolHeaderLengthCount;
    QList<QString> protocolList;
    QHash<QString,position_t> protocolPositionHash;

    QString summery;

    /*保留字段，用于底层解析结果向高层解析结果传送数据*/
    struct reserve{
        void *ptr;
        qint32 len;
    };

    /*
     * 1.对保留字段的添加操作必须在AddNextLayer之前进行
     * 2.对保留字段的取操作，只需要保证在被清理之前使用就可以了
     * 3.不需要的数据要及时清理，不要轻易使用ClearReserveXX()方法，以免清理掉意想不到的数据
     */
    QHash<QString,struct reserve> reserve_ptr;
    QHash<QString,qint64> reserve_val;
};

#endif // DISSECTRESULTBASE_H
