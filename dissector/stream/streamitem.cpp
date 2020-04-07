#include "streamitem.h"

StreamItem::Position_Direction::Position_Direction(qint64 dissRes_index){
    dissRes_index_hash.insert(true,new QList<qint64>);
    dissRes_index_hash.value(true)->append(dissRes_index);
}

bool StreamItem::Position_Direction::Modify(qint64 dissRes_index,bool direction){
    bool add_new_list = false;
    if(!dissRes_index_hash.contains(direction)){
        dissRes_index_hash.insert(direction,new QList<qint64>);
        add_new_list = true;
    }
    dissRes_index_hash.value(direction)->append(dissRes_index);
    return add_new_list;
}

QHash<bool,QList<qint64>*> StreamItem::Position_Direction::GetPD(){
    return this->dissRes_index_hash;
}

StreamItem::StreamItem(QString addressA,QString addressB,quint32 portA,quint32 portB,qint64 dissRes_index,qint64 *streamIndex){
    this->addressA.append(addressA);
    this->addressB.append(addressB);
    this->portA = portA;
    this->portB = portB;
    this->streamIndexAB = *streamIndex;
    (*streamIndex)++;
    this->streamIndexBA = -1;
    this->pd = new Position_Direction(dissRes_index);
}

void StreamItem::Modify(qint64 dissRes_index, QString addressS,qint64 *streamIndex){
    bool direction;
    if(addressA.compare(addressS) == 0){
        direction = true;
    }else{
        direction = false;
    }
    if(this->pd->Modify(dissRes_index,direction)){
        this->streamIndexBA = *streamIndex;
        (*streamIndex)++;
    }
}

qint64 StreamItem::GetStreamIndex(QString addressS){
    if(addressS.compare(this->addressA) == 0)
        return this->streamIndexAB;
    else
        return this->streamIndexBA;
}

QList<qint64>* StreamItem::GetPositions(bool AB){
    return this->pd->GetPD().value(AB);
}

qint64 StreamItem::GetPositionsCount(bool AB){
    return this->pd->GetPD().value(AB)->length();
}

