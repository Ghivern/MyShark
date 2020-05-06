#ifndef BIT_H
#define BIT_H
#include <QtCore>

class Bit
{
public:
    Bit();
    Bit(quint64 data);

    Bit& Set(quint64 data);
    quint8 GetBit(qint32 position);

    static quint8 GetBitFromOctetPtr(quint8 *data,qint32 position);

    static QString GetDotStrFormOctetPtr(quint8 *data,qint32 star = -1,qint32 end = -1);

private:
    quint64 data;
};

#endif // BIT_H
