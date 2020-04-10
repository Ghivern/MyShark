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