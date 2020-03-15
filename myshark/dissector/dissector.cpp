#include "dissector.h"

Dissector::Dissector(Capturer *capturer,Loader *loader)
{
    this->loader = loader;
    this->capturer = capturer;
    this->dissResList = capturer->GetDissResList();
}

Dissector::Dissector(Capturer *capture){
    this->loader = new Loader();
    this->capturer = capture;
    this->dissResList = capturer->GetDissResList();
}

DissRes* Dissector::GetDissResByIndex(qint64 index){
    return this->dissResList->at(index);
}

Loader* Dissector::GetLoader(){
    return this->loader;
}

DissResList* Dissector::GetDissResList(){
    return this->dissResList;
}

void Dissector::Dissect(qint64 index){
    //通过Loader进行解析
    qDebug() << "Dissector : Dissect" ;
    this->loader->GetDissector(1)->Dissect(this->dissResList,index);
    emit onePacketDissected(index);
}
