#include "device.h"

QList<pcap_if_t*> Device::devices;
QList<QString> Device::devNames;


Device::Device()
{
    pcap_if_t *devLinkList;
    char errbuf[PCAP_ERRBUF_SIZE];
    if(pcap_findalldevs(&devLinkList,errbuf) == -1){
        qDebug() << QLatin1String(errbuf);
    }else{
        while(devLinkList != NULL){
            this->devices.append(devLinkList);
            this->devNames.append(QLatin1String(devLinkList->name));
            devLinkList = devLinkList->next;
        }
    }
}

Device::~Device(){
    if(!this->devices.isEmpty())
        pcap_freealldevs(this->devices.first());
}

qint32 Device::GetDeviceCount(){
    return this->devices.length();
}

//Static Methods
qint32 Device::GetDeviceIndexByName(QString devName){
    return Device::devNames.indexOf(devName);
}

QString Device::GetDeviceNameByIndex(qint32 index){
    return Device::devNames.at(index);
}
