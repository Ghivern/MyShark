#include "capturer.h"


Capturer::Capturer(QString devName,QHash<QString,quint64>* dissectorOptions)
{
    this->capHandle = new CapHandle(devName);
    this->capHandle->ActivateHandleWithParas();
    this->dissectorOptions = dissectorOptions;
//    this->dissResList = new DissResList_t;
//    this->mutex = new QMutex();
    this->stop = false;
}

Capturer::Capturer(CapHandle *capHandle,QHash<QString,quint64>* dissectorOptions){
    this->capHandle = capHandle;
    this->capHandle->ActivateHandleWithParas();
    this->dissectorOptions = dissectorOptions;
//    this->dissResList = new DissResList_t;
//    this->mutex = new QMutex();
    this->stop = false;
}

Capturer::~Capturer(){
//    this->capHandle->Close();
//    delete capHandle;
    //delete mutex;
//    for(int index = 0; index < this->dissResList->length(); index++)
//        delete dissResList->at(index);
//    delete dissResList;
}

//qint32 Capturer::GetIntLinkType(){
//    return this->capHandle->GetLinkType();
//}

//QList<DissRes*>* Capturer::GetDissResList(){
//    return this->dissResList;
//}

qint64 Capturer::GetCount(){
    return this->dissectResultFrameList.length();
}

DissectResultFrame* Capturer::GetDissectResultFrameByIndex(qint64 index){
    return this->dissectResultFrameList.at(index);
}

CapHandle* Capturer::GetCapHandle(){
    return this->capHandle;
}

void Capturer::run(){
    const uchar *raw;
    struct pcap_pkthdr *pkthdr;
    qint64 index = 0;
    qint32 res;
    QList<void*> *reserves = new QList<void*> {dissectorOptions};
    //this->dissectResultFrameList.clear();
    while(!this->stop){
        if((res = pcap_next_ex(this->capHandle->GetPcapHandle(),&pkthdr,&raw)) == 1){
            qDebug() << "抓取成功";
            switch (this->capHandle->GetLinkType()) {
            case 1:
            {
                //dissRes = new DissResEth(index);
                this->dissectResultFrameList.append(new DissectResultFrame(raw,pkthdr,index
                    ,DissectResultFrame::PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY,reserves));
//                if( this->stop ) break;
                emit onePacketCaptured(this->dissectResultFrameList.at(index));
                index++;
                break;
            }
            default:
            {
                qDebug() << "未知链路层类型";
//                dissRes = new DissRes(index);
                break;
            }
            }

        }else if(res == 0){
            qDebug() << "Capturer : timeout";
        }else if(res == -1){
            qDebug() << "Capturer : error, 没有权限";
        }else if(res == -2){
            qDebug() << "Capturer : come to file end";
        }else{
            qDebug() << "Other error";
        }
    }
    this->exit();
}


//Public Slots
void Capturer::Start(){
    this->stop = false;
    this->start();
}

void Capturer::Stop(){
    this->stop = true;
}


