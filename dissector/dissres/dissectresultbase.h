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

    void SetData(quint8 *data);
    void SetSummery(QString summery);

    const quint8* GetData();
    qint64 GetIndex();
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

    quint8 *data;
    qint64 index;
    qint32 protocolHeaderLengthCount;
    QList<QString> protocolList;
    QHash<QString,position_t> protocolPositionHash;

    QString summery;
};

#endif // DISSECTRESULTBASE_H
