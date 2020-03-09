#include "dissector.h"

Dissector::Dissector(Capturer *capturer,Loader *loader)
{
    this->dissResList = new dissResList_t;
    this->loader = loader;
    this->capturer = capturer;
}

Dissector::~Dissector(){
    for(qint64 index = 0; index < this->dissResList->length(); index++){
        delete this->dissResList->at(index);
    }
}

DissRes* Dissector::GetDissResByIndex(qint64 index){
    return this->dissResList->at(index);
}

Loader* Dissector::GetLoader(){
    return this->loader;
}

dissResList_t* Dissector::GetDissResList(){
    return this->dissResList;
}

void Dissector::Dissect(qint64 index){
    //通过Loader进行解析
    qDebug() << "Dissector : Dissect" ;
    this->loader->GetDissector(1)->Dissect(this->capturer->GetRawList(),this->dissResList,index);
    emit onePacketDissected(index);
}
