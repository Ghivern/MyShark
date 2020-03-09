#ifndef DEVICE_H
#define DEVICE_H
#include <QList>
#include "pcap.h"
#include "../../global/global.h"

class Device
{
public:
    Device();
    ~Device();

    qint32 GetDeviceCount();

    static qint32 GetDeviceIndexByName(QString devName);
    static QString GetDeviceNameByIndex(qint32 index);


private:
    static QList<pcap_if_t*> devices;
    static QList<QString> devNames;
};

#endif // DEVICE_H