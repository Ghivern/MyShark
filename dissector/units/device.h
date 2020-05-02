#ifndef DEVICE_H
#define DEVICE_H
#include <QList>
#include "pcap.h"

#include <QDebug>

class Device
{
public:
    Device();
    ~Device();

    qint32 GetDeviceCount();
    QStringList GetDevices();

    ;
    static qint32 GetDeviceIndexByName(QString devName);
    static QString GetDeviceNameByIndex(qint32 index);


private:
    static void updateLists();

    static QList<pcap_if_t*> devices;
    static QList<QString> devNames;
    static bool first;
};

#endif // DEVICE_H
