#ifndef DISSECTRESULTBASE_H
#define DISSECTRESULTBASE_H

#include <QList>
#include <QHash>

class DissectResultBase
{
public:
    DissectResultBase(const quint8 *data, qint64 index);
    ~DissectResultBase();

    void UpdateProtocolHeaderLengthCount(qint32 headerLength);
    void PushToProtocolList(QString protocolName, qint32 protocolHeaderLength, bool hide = false);
    void UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength);

    const quint8* GetData();
    qint64 GetIndex();
    qint32 GetProtocolHeaderLengthCount();
    qint32 GetProtocolListLength();
    QString GetProtocolNameByIndex(qint32 index);
    const quint8* GetProtocolHeaderStartPtrByName(QString protocolName);

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
};

#endif // DISSECTRESULTBASE_H
