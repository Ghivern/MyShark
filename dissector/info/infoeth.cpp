#include "infoeth.h"

InfoEth::InfoEth(qint32 devIndex,qint32 datalink)
{
    this->devIndex = devIndex;
    this->datalink = datalink;
}

qint32 InfoEth::GetDatalink(){
    return this->datalink;
}

qint32 InfoEth::GetDevIndex(){
    return this->devIndex;
}

QString InfoEth::GetDevName(){
    return Device::GetDeviceNameByIndex(this->devIndex);
}

QString InfoEth::GetDatalinkName(){
    return pcap_datalink_val_to_name(this->datalink);
}
