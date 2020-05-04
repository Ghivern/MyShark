#include "caphandle.h"

//CapHandle::CapHandle()
//{
//    this->pcapHandle = nullptr;
//    this->devIndex = -1;
//    this->devName = "";
//}

CapHandle::CapHandle(QString devNameOrPath,bool fromFile){
    this->fromFile = fromFile;
    this->devNameOrPath = devNameOrPath;
    if( !fromFile )
        this->devIndex = Device::GetDeviceIndexByName(devNameOrPath);
    else
        this->devIndex = -1;
    this->createHandle();
}

void CapHandle::ChangeDevice(QString devName){
    if( !devName.isEmpty() ){
        this->devNameOrPath = devName;
        this->devIndex = Device::GetDeviceIndexByName(devName);
    }
    this->fromFile = false;
    this->closeOldHandle();
    this->createHandle();
}

void CapHandle::ChangePath(QString path){
    if( !path.isEmpty() ){
        this->devNameOrPath = path;
        this->devIndex = -1;
    }
    this->fromFile = true;
    this->closeOldHandle();
    this->createHandle();
}

//CapHandle::CapHandle(qint32 devIndex){
//    char errbuf[PCAP_ERRBUF_SIZE];
//    this->error.clear();
//    this->pcapHandle = pcap_create(devName.toLatin1(),errbuf);
//    if( this->pcapHandle == NULL ){
//        this->error.append(QLatin1String(errbuf));
//    }else{
//        this->devName = Device::GetDeviceNameByIndex(devIndex);
//        this->devIndex = devIndex;
//    }
//}

//void CapHandle::createPcapHandle(QString devName){
//    char errbuf[PCAP_ERRBUF_SIZE];
//    this->error.clear();
//    this->pcapHandle = pcap_create(devName.toLatin1(),errbuf);
//    if( this->pcapHandle == NULL ){
//        this->error.append(QLatin1String(errbuf));
//    }else{
//        this->devName = devName;
//        this->devIndex = Device::GetDeviceIndexByName(devName);
//    }
//}

//void CapHandle::createPcapHandle(qint32 devIndex){
//    char errbuf[PCAP_ERRBUF_SIZE];
//    this->error.clear();
//    this->pcapHandle = pcap_create(devName.toLatin1(),errbuf);
//    if( this->pcapHandle == NULL ){
//        this->error.append(QLatin1String(errbuf));
//    }else{
//        this->devName = Device::GetDeviceNameByIndex(devIndex);
//        this->devIndex = devIndex;
//    }
//}

void CapHandle::SetSnaplen(qint32 snap){
    qint32 status = pcap_set_snaplen(this->pcapHandle,snap);
    if( status > 0 )
        throw QString(pcap_statustostr(status));
    if( status < 0 )
        throw QString(pcap_geterr(this->pcapHandle));
}

void CapHandle::SetPromisc(qint32 promisc){
    qint32 status =  pcap_set_promisc(this->pcapHandle,promisc);
    if( status > 0)
        throw QString(pcap_statustostr(status));
    if( status < 0 )
        throw QString(pcap_geterr(this->pcapHandle));
}

void CapHandle::SetImmediateMode(qint32 immediateMode){
    qint32 status = pcap_set_immediate_mode(this->pcapHandle,immediateMode);
    if( status > 0 )
        throw QString(pcap_statustostr(status));
    if( status < 0 )
        throw QString(pcap_geterr(this->pcapHandle));
}

void CapHandle::SetNonBlock(qint32 nonblock){
    char errbuf[PCAP_ERRBUF_SIZE];
    qint32 status = pcap_setnonblock(this->pcapHandle,nonblock,errbuf);
    if( status > 0 )
        throw QString(errbuf);
    if( status < 0 )
        throw QString(pcap_geterr(this->pcapHandle));
}

void CapHandle::ActivateHandle(){
    qint32 status = pcap_activate(this->pcapHandle);
    if( status > 0 )
        throw QString(pcap_statustostr(status));
    if( status < 0 )
        throw QString(pcap_geterr(this->pcapHandle));
}

void CapHandle::ActivateHandleWithParas(qint32 promisc
                                        , qint32 immediateMode
                                        //,qint32 nonblock
                                        ,qint32 snapLen){
    this->SetPromisc(promisc);
    this->SetSnaplen(snapLen);
    this->SetImmediateMode(immediateMode);
    this->ActivateHandle();
    //this->SetNonBlock(nonblock);
}

void CapHandle::Close(){
    pcap_close(this->pcapHandle);
}

bool CapHandle::FromFile(){
    return this->fromFile;
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
    if( fromFile )
        return "unknow";
    return this->devNameOrPath;
}

qint32 CapHandle::GetDeviceIndex(){
    return this->devIndex;
}

void CapHandle::closeOldHandle(){
    pcap_close(this->pcapHandle);
}

void CapHandle::createHandle(){
    char errbuf[PCAP_ERRBUF_SIZE];
    if( !fromFile )
        this->pcapHandle = pcap_create(this->devNameOrPath.toLatin1(),errbuf);
    else
        this->pcapHandle = pcap_open_offline(this->devNameOrPath.toLatin1(),errbuf);
    if( this->pcapHandle == nullptr )
        throw QString(errbuf);
}
