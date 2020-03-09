#ifndef INFOETH_H
#define INFOETH_H
#include "info.h"

class InfoEth
{
public:
    InfoEth(qint32 devIndex,qint32 datalink);
    qint32 GetDevIndex();
    QString GetDevName();
    qint32 GetDatalink();
    QString GetDatalinkName();

private:
    qint32 devIndex;
    qint32 datalink;
};

#endif // INFOETH_H
