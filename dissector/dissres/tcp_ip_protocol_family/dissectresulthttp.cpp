#include "dissectresulthttp.h"

DissectResultHttp::DissectResultHttp(DissectResultBase *dissectResultBase,void *reserves){
    Q_UNUSED(reserves)
    this->nextLayer = nullptr;
    this->dissectResultBase = dissectResultBase;

    dissectResultBase->PushToProtocolList("http",0);
}

void* DissectResultHttp::GetNextLayer(){
    return this->nextLayer;
}

DissectResultBase* DissectResultHttp::GetDissectResultBase(){
    return this->dissectResultBase;
}
