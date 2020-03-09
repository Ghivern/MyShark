#include "info.h"

Info::Info(qint32 devIndex,qint32 datalink)
{
    this->devIndex = devIndex;
    this->datalink = datalink;
}

qint32 Info::GetDatalink(){
    return this->datalink;
}

qint32 Info::GetDevIndex(){
    return this->devIndex;
}

QString Info::GetDevName(){
    return Device::GetDeviceNameByIndex(this->devIndex);
}

QString Info::GetDatalinkName(){
    return pcap_datalink_val_to_name(this->datalink);
}
