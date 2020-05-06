#include "bit.h"

Bit::Bit()
{
    this->data = 0;
}

Bit::Bit(quint64 data){
    this->data = data;
}

Bit& Bit::Set(quint64 data){
    this->data = data;
    return *this;
}

quint8 Bit::GetBit(qint32 position){
    quint64 temp = (this->data) << (sizeof(quint64) * 8 - 1 - position);
    temp >>= (sizeof(quint64) * 8 - 1);
    return (quint8)temp;
}

quint8 Bit::GetBitFromOctetPtr(quint8 *data, qint32 position){
    quint8 temp = *data << (7 - position);
    return temp >> 7;
}

QString Bit::GetDotStrFormOctetPtr(quint8 *data,qint32 start,qint32 end){
    if( start == - 1 && end == -1)
        return QString(".... ....");

    QString res = "";
    for( qint32 index = 7; index > end; index--)
        res.append(".");

    for( qint32 index = end; index >= start; index-- )
        res.append(QString::asprintf("%d",Bit::GetBitFromOctetPtr(data,index)));

    for( qint32 index = start - 1; index >= 0; index--)
        res.append(".");


    res.insert(4," ");

    return res;
}
