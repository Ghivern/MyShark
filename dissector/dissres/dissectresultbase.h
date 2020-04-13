#ifndef DISSECTRESULTBASE_H
#define DISSECTRESULTBASE_H

#include <QList>
#include <QHash>

/*
 * 主要功能就是获取各层协议首部的起始地址
 */
class DissectResultBase
{
public:
    DissectResultBase(const quint8 *data, qint64 index);
    ~DissectResultBase();

    void UpdateProtocolHeaderLengthCount(qint32 headerLength);
    void PushToProtocolList(QString protocolName, qint32 protocolHeaderLength);
    void UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength);

    void SetSummery(QString summery);

    const quint8* GetData();
    quint64 GetIndex();
    qint32 GetProtocolHeaderLengthCount();
    qint32 GetProtocolListLength();
    QString GetProtocolNameByIndex(qint32 index);
    const quint8* GetProtocolHeaderStartPtrByName(QString protocolName);
    QString GetSummery();

private:
    typedef struct position_t{
        qint32 start;
        qint32 end;
    }position_t;

    const quint8 *data;
    qint64 index;
    qint32 protocolHeaderLengthCount;
    QList<QString> protocolList;
    QHash<QString,position_t> protocolPositionHash;

    QString summery;

    /*保留字段，用于底层解析结果向高层解析结果传送数据*/
    QHash<QString,quint64> reserveHash;
    void *reserve;
};

#endif // DISSECTRESULTBASE_H
