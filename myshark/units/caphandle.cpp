#include "caphandle.h"

CapHandle::CapHandle()
{
    this->pcapHandle = NULL;
    this->devIndex = -1;
    this->devName = "";
}

CapHandle::CapHandle(QString devName){
    char errbuf[PCAP_ERRBUF_SIZE];
    this->error.clear();
    this->pcapHandle = pcap_create(devName.toLatin1(),errbuf);
    if( this->pcapHandle == NULL ){
        this->error.append(QLatin1String(errbuf));
    }else{
        this->devName = devName;
        this->devIndex = Device::GetDeviceIndexByName(devName);
    }
}

CapHandle::CapHandle(qint32 devIndex){
    char errbuf[PCAP_ERRBUF_SIZE];
    this->error.clear();
    this->pcapHandle = pcap_create(devName.toLatin1(),errbuf);
    if( this->pcapHandle == NULL ){
        this->error.append(QLatin1String(errbuf));
    }else{
        this->devName = Device::GetDeviceNameByIndex(devIndex);
        this->devIndex = devIndex;
    }
}

void CapHandle::createPcapHandle(QString devName){
    char errbuf[PCAP_ERRBUF_SIZE];
    this->error.clear();
    this->pcapHandle = pcap_create(devName.toLatin1(),errbuf);
    if( this->pcapHandle == NULL ){
        this->error.append(QLatin1String(errbuf));
    }else{
        this->devName = devName;
        this->devIndex = Device::GetDeviceIndexByName(devName);
    }
}

void CapHandle::createPcapHandle(qint32 devIndex){
    char errbuf[PCAP_ERRBUF_SIZE];
    this->error.clear();
    this->pcapHandle = pcap_create(devName.toLatin1(),errbuf);
    if( this->pcapHandle == NULL ){
        this->error.append(QLatin1String(errbuf));
    }else{
        this->devName = Device::GetDeviceNameByIndex(devIndex);
        this->devIndex = devIndex;
    }
}

qint32 CapHandle::SetSnaplen(qint32 snap){
    return pcap_set_snaplen(this->pcapHandle,snap);
}

qint32 CapHandle::SetPromisc(qint32 promisc){
    return pcap_set_promisc(this->pcapHandle,promisc);
}

qint32 CapHandle::SetImmediateMode(qint32 immediateMode){
    return pcap_set_immediate_mode(this->pcapHandle,immediateMode);
}

qint32 CapHandle::ActivateHandle(){
    this->error.clear();
    if( pcap_activate(this->pcapHandle) < 0){
        this->error.append(QLatin1String(pcap_geterr(this->pcapHandle)));
        return -1;
    }
    pcap_setnonblock(this->pcapHandle,1,nullptr);
    return 0;
}

qint32 CapHandle::ActivateHandleWithParas(qint32 promisc, qint32 immediateMode,qint32 snapLen){
    this->SetPromisc(promisc);
    this->SetSnaplen(snapLen);
    this->SetImmediateMode(immediateMode);
    //pcap_set_timeout(this->pcapHandle,1);
    return this->ActivateHandle();
}

pcap_t* CapHandle::GetPcapHandle(){
    return this->pcapHandle;
}

qint32 CapHandle::GetLinkType(){
    return pcap_datalink(this->pcapHandle);
}

QString CapHandle::GetLinkTypeName(){
    return pcap_datalink_val_to_name( pcap_datalink(this->pcapHandle) );
}

QString CapHandle::GetLinkTypeDes(){
    return pcap_datalink_val_to_description( pcap_datalink(this->pcapHandle) );
}

QString CapHandle::GetDeviceName(){
    return this->devName;
}

qint32 CapHandle::GetDeviceIndex(){
    return this->devIndex;
}

QString CapHandle::GetError(){
    return this->error;
}

void CapHandle::Close(){
    pcap_close(this->pcapHandle);
}
