#ifndef INFO_H
#define INFO_H
#include <QtCore>
#include "../units/device.h"

class Info
{
public:
    Info(qint32 devIndex,qint32 datalink);
    qint32 GetDevIndex();
    QString GetDevName();
    qint32 GetDatalink();
    QString GetDatalinkName();

private:
    qint32 devIndex;
    qint32 datalink;
};

#endif // INFO_H
