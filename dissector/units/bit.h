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

private:
    quint64 data;
};

#endif // BIT_H
