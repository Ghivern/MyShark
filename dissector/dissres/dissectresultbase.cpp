#include "dissectresultbase.h"

DissectResultBase::DissectResultBase(const quint8 *data, qint64 index){
    this->data = data;
    this->index = index;
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

void DissectResultBase::SetData(quint8 *data){
    this->data = data;
}

void DissectResultBase::SetSummery(QString summery){
    this->summery.clear();
    this->summery.append(summery);
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
    return this->data + this->protocolPositionHash.value(protocolName).start;
}

QString DissectResultBase::GetSummery(){
    return this->summery;
}
