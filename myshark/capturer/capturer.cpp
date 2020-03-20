#include "capturer.h"

Capturer::Capturer(QString devName)
{
    this->capHandle = new CapHandle(devName);
    this->capHandle->ActivateHandleWithParas();
    this->dissResList = new DissResList;
    this->mutex = new QMutex();
    this->stop = false;
}

Capturer::~Capturer(){
    delete capHandle;
    delete mutex;
    for(int index = 0; index < this->dissResList->length(); index++)
        delete dissResList->at(index);
    delete dissResList;
}

QList<DissRes*>* Capturer::GetDissResList(){
    return this->dissResList;
}

void Capturer::run(){
    const uchar *raw;
    struct pcap_pkthdr *pkthdr;
    qint64 index = 0;
    qint32 res;
    while(!this->stop){
        if((res = pcap_next_ex(this->capHandle->GetPcapHandle(),&pkthdr,&raw)) == 1){
            DissRes *dissRes;
            switch (this->capHandle->GetLinkType()) {
            case 1:
            {
                dissRes = new DissResEth(index);
                break;
            }
            default:
            {
                qDebug() << "未知链路层类型";
                dissRes = new DissRes(index);
                break;
            }
            }
            dissRes->SetPacket(raw,pkthdr);
            this->mutex->lock();
            this->dissResList->append(dissRes);
            this->mutex->unlock();
            emit onePacketCaptured(index);
            qDebug() << "Capturer : capture one packet successfully";
            index++;
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
}


//Public Slots
void Capturer::Start(){
    this->start();
}

void Capturer::Stop(){
    this->stop = false;
}
