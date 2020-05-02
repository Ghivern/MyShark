#include "device.h"

QList<pcap_if_t*> Device::devices;
QList<QString> Device::devNames;
bool Device::first = true;


Device::Device()
{
    if( first ){
        this->updateLists();
        Device::first = false;
    }
}

Device::~Device(){
}

qint32 Device::GetDeviceCount(){
    return this->devices.length();
}

QStringList Device::GetDevices(){
    return QStringList(this->devNames);
}

//Static Methods
qint32 Device::GetDeviceIndexByName(QString devName){
    if( first ){
        Device::updateLists();
        Device::first = false;
    }
    return Device::devNames.indexOf(devName);
}

QString Device::GetDeviceNameByIndex(qint32 index){
    if( first ){
        Device::updateLists();
        Device::first = false;
    }
    if(index < Device::devNames.length())
        return Device::devNames.at(index);
    else
        return "";
}

void Device::updateLists(){
    pcap_if_t *devLinkList;
    char errbuf[PCAP_ERRBUF_SIZE];
    if(pcap_findalldevs(&devLinkList,errbuf) == -1){
        throw QString(errbuf);
    }else{
        Device::devices.clear();
        Device::devNames.clear();
        while(devLinkList != nullptr){
            Device::devices.append(devLinkList);
            Device::devNames.append(QLatin1String(devLinkList->name));
            devLinkList = devLinkList->next;
        }
        pcap_freealldevs(devLinkList);
    }
}
