#include "capturer.h"


Capturer::Capturer(QString devName,QHash<QString,quint64>* dissectorOptions)
{
    this->capHandle = new CapHandle(devName);
    this->capHandle->ActivateHandleWithParas();
    this->dissectorOptions = dissectorOptions;
//    this->dissResList = new DissResList_t;
//    this->mutex = new QMutex();

    this->tempfile = new QTemporaryFile;
    this->tempfile->setAutoRemove(true);
    //this->tempfile.setFileTemplate("myshark_" + devName + "_XXXXXX");
    this->tempfile->open();
    this->tempfile->close();
    this->dumper = new Dumper(this->capHandle,this->tempfile->fileName());
    //qDebug() << this->tempfile.fileName();
    //qDebug() << QDir::tempPath();
    this->stop = false;
}

Capturer::Capturer(CapHandle *capHandle,QHash<QString,quint64>* dissectorOptions){
    this->capHandle = capHandle;
    //this->capHandle->ActivateHandleWithParas();
    this->dissectorOptions = dissectorOptions;
//    this->dissResList = new DissResList_t;
//    this->mutex = new QMutex();
    this->tempfile = new QTemporaryFile;
    this->tempfile->setAutoRemove(true);
    //this->tempfile.setFileTemplate("/myshark_" + capHandle->GetDeviceName() + "_XXXXXX");
    this->tempfile->open();
    this->tempfile->close();
    this->dumper = new Dumper(this->capHandle,this->tempfile->fileName());
    //qDebug() << this->tempfile.fileName();
    //qDebug() << QDir::tempPath();
    this->stop = false;
}

Capturer::~Capturer(){
    //this->tempfile.deleteLater();
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

QTemporaryFile* Capturer::GetTempFile(){
    return this->tempfile;
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
            if( this->stop ) { break; }
//            qDebug() << "抓取成功";
            switch (this->capHandle->GetLinkType()) {
            case 1:
            {
                //dissRes = new DissResEth(index);
                this->dissectResultFrameList.append(new DissectResultFrame(raw,pkthdr,index
                    ,DissectResultFrame::PROTOCOL_FAMILY_TYPE::TCP_IP_PROTOCOL_FAMILY,reserves));
                this->dumper->Dump(pkthdr,raw);
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
            break;
        }else{
            qDebug() << pcap_geterr(this->capHandle->GetPcapHandle());
        }
    }
    this->dumper->Flush();
    this->dumper->Close();
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


