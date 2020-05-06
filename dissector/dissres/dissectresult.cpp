#include "dissectresult.h"

DissectResult::DissectResult(DissectResultBase *dissectResultBase)
{
    this->nextLayer = nullptr;
    this->dissectResultBase = dissectResultBase;
    this->streamIndexPlusOne = 0;
}

void DissectResult::SetStremIndexPlusOne(qint64 streamIndexPlusOne){
    this->streamIndexPlusOne = streamIndexPlusOne;
}

void DissectResult::SetNextLayer(void *nextlayer){
    this->nextLayer = nextlayer;
}

qint64 DissectResult::GetStreamIndex(){
    return qAbs(this->streamIndexPlusOne) - 1;
}

qint64 DissectResult::GetStreamIndexPlusOne(){
    return this->streamIndexPlusOne;
}

bool DissectResult::ContainNextLayer(){
    if( this->nextLayer == nullptr )
        return false;
    return true;
}

void* DissectResult::GetNextLayer(){
    return this->nextLayer;
}

DissectResultBase* DissectResult::GetDissectResultBase(){
    return this->dissectResultBase;
}
