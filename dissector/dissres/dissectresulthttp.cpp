#include "dissectresulthttp.h"

DissectResultHttp::DissectResultHttp(DissectResultBase *dissectResultBase){
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
