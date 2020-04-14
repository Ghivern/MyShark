#include "dissector.h"

Dissector::Dissector(DissResList_t* dissResList, int linkType){
    this->dissResList = dissResList;
    this->linkType = linkType;
    this->loader = new Loader();
}

DissRes* Dissector::GetDissResByIndex(qint64 index){
    return this->dissResList->at(index);
}

Loader* Dissector::GetLoader(){
    return this->loader;
}

DissResList_t* Dissector::GetDissResList(){
    return this->dissResList;
}

void Dissector::Dissect(qint64 index){
//    //通过Loader 查找相应链路层类型的解析器
//    qDebug() << this->linkType;
//    this->loader->GetDissector(this->linkType)->Dissect(this->dissResList,index);
//    qDebug() << "解析器调用完成";
    emit onePacketDissected(index);
}
