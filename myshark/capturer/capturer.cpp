#include "capturer.h"

Capturer::Capturer(QString devName)
{
    this->capHandle = new CapHandle(devName);
    this->capHandle->ActivateHandleWithParas();
    this->rawList = new QList<raw_t*>;
    this->mutex = new QMutex();
    this->stop = false;
}

Capturer::~Capturer(){
    delete capHandle;
    delete rawList;
    delete mutex;
    for(int index = 0; index < this->rawList->length(); index++){
        delete this->rawList->at(index)->raw;
        delete this->rawList->at(index)->pkthdr;
        delete this->rawList->at(index);
    }
}

rawList_t* Capturer::GetRawList(){
    return this->rawList;
}

void Capturer::run(){
    const uchar *raw;
    struct pcap_pkthdr *pkthdr;
    raw_t *rawAndPkthdr;
    qint64 index = 0;
    qint32 res;
    while(!this->stop){
        if((res = pcap_next_ex(this->capHandle->GetPcapHandle(),&pkthdr,&raw)) == 1){
            struct pcap_pkthdr *newPkthdr = new struct pcap_pkthdr;
            memcpy(newPkthdr,pkthdr,sizeof (struct pcap_pkthdr));
            uchar *newRaw = new uchar[pkthdr->caplen];
            memcpy(newRaw,raw,pkthdr->caplen);
            rawAndPkthdr = new raw_t;
            rawAndPkthdr->raw = newRaw;
            rawAndPkthdr->pkthdr = newPkthdr;
            this->mutex->lock();
            this->rawList->append(rawAndPkthdr);
            emit onePacketCaptured(index);
            qDebug() << "Capturer : capture one packet successfully";
            this->mutex->unlock();
            index++;
        }else if(res == 0){
            qDebug() << "Capturer : timeout";
        }else if(res == -1){
            qDebug() << "Capturer : error";
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
