#include "dissectresultbase.h"

DissectResultBase::DissectResultBase(const quint8 *data,const pcap_pkthdr *pkther, qint64 index){
    this->data = data;
    this->index = index;
    this->pkthdr = pkther;
}

void DissectResultBase::UpdateProtocolHeaderLengthCount(qint32 headerLength){
    this->protocolHeaderLengthCount += headerLength;
}

void DissectResultBase::PushToProtocolList(QString protocolName, qint32 protocolHeaderLength){
    position_t position;
    if(this->protocolList.isEmpty())
        position.start = 0;
            else
        position.start = this->protocolPositionHash.value(this->protocolList.last()).end;
    position.end = position.start + protocolHeaderLength;
    this->protocolPositionHash.insert(protocolName,position);

    this->protocolList.append(protocolName);
}

void DissectResultBase::UpdateProtocolList(QString protocolName, qint32 newProtocolHeaderLength){
    position_t position = this->protocolPositionHash.value(protocolName);
    qint32 oldProtocolHeaderLength = position.end - position.start;
    qint32 offset = newProtocolHeaderLength - oldProtocolHeaderLength;

    position.end = position.start + newProtocolHeaderLength;
    this->protocolPositionHash.remove(protocolName);
    this->protocolPositionHash.insert(protocolName,position);

    for(qint32 i = this->protocolList.indexOf(protocolName) + 1; i < GetProtocolListLength(); i++){
        position_t position = this->protocolPositionHash.value(GetProtocolNameByIndex(i));
        position.start += offset;
        position.end += offset;
        this->protocolPositionHash.remove(this->GetProtocolNameByIndex(i));
        this->protocolPositionHash.insert(this->GetProtocolNameByIndex(i),position);
    }

}

void DissectResultBase::SetSummery(QString summery){
    this->summery.clear();
    this->summery.append(summery);
}

const quint8* DissectResultBase::GetData(){
    return this->data;
}

const pcap_pkthdr* DissectResultBase::GetPkthdr(){
    return this->pkthdr;
}

quint64 DissectResultBase::GetIndex(){
    return this->index;
}

qint32 DissectResultBase::GetProtocolHeaderLengthCount(){
    return this->protocolHeaderLengthCount;
}

qint32 DissectResultBase::GetProtocolListLength(){
    return this->protocolList.length();
}

QString DissectResultBase::GetProtocolNameByIndex(qint32 index){
    if(index < this->GetProtocolListLength())
        return this->protocolList.at(index);
    else
        return "";
}

const quint8* DissectResultBase::GetProtocolHeaderStartPtrByName(QString protocolName){
    if(this->protocolPositionHash.contains(protocolName))
        return this->data + this->protocolPositionHash.value(protocolName).start;
    else
        return NULL;
}

const quint8* DissectResultBase::GetProtocolHeaderEndPtrByName(QString protocolName){
    if(this->protocolPositionHash.contains(protocolName))
        return this->data + this->protocolPositionHash.value(protocolName).end;
    else
        return NULL;
}

QString DissectResultBase::GetSummery(){
    return this->summery;
}

/*保留字段的操作*/
void DissectResultBase::ClearReserve(){
    this->reserve_ptr.clear();
    this->reserve_val.clear();
}

void DissectResultBase::ClearReserveVal(){
    this->reserve_val.clear();
}

void DissectResultBase::ClearReservePtr(){
    this->reserve_ptr.clear();
}

void DissectResultBase::AddAdditional(QString name, qint64 val){
    this->reserve_val.insert(name,val);
}

void DissectResultBase::AddAdditional(QString name, void *ptr, qint32 dataLen){
    struct reserve res;
    res.ptr = (void*)ptr;
    res.len = dataLen;
    this->reserve_ptr.insert(name,res);
}

void DissectResultBase::AddAdditional(QString name, qint64 val, void *ptr, qint32 dataLen){
    struct reserve res;
    res.ptr = ptr;
    res.len = dataLen;
    this->reserve_ptr.insert(name,res);
    this->reserve_val.insert(name,val);
}

void DissectResultBase::RemoveAdditional(QString name){
    this->reserve_ptr.remove(name);
    this->reserve_val.remove(name);
}

void DissectResultBase::RemoveAdditionalVal(QString name){
    this->reserve_val.remove(name);
}

void DissectResultBase::RemoveAdditionalPtr(QString name){
    this->reserve_ptr.remove(name);
}

qint64 DissectResultBase::GetAdditionalVal(QString name){
    if(!this->reserve_val.contains(name))
        return -1;
    return this->reserve_val.value(name);
}

void* DissectResultBase::GetAdditionalPtr(QString name, qint32 *len){
    if(!this->reserve_ptr.contains(name))
        return NULL;
    if(len!=NULL)
        *len = this->reserve_ptr.value(name).len;
    return this->reserve_ptr.value(name).ptr;
}
