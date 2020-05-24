#include "dissectresultbase.h"


QHash<QString,quint64>* DissectResultBase::dissectorOptions = new QHash<QString,quint64>
      {
            {"frame",
                 FRAME_SHOW_NUMBER_OF_BITS | FRAME_GENERATE_EPOCH_TIME
            },

            {"ether",
                0
            },

            {"arp",
                0
            },

            {"ipv4",
                0//IPV4_VALIDATE_CHECKSUM
            },

            {"tcp",
                TCP_ANALYZE_TCP_SEQUENCE_NUMBER | TCP_RELATIVE_SEQUENCE_NUMBER
            },

            {"udp",
                0
            }
      };

qint32 DissectResultBase::interfaceId = -1;
QString DissectResultBase::interfaceName = "";
qint32 DissectResultBase::linklayerType = -1;
QString DissectResultBase::linklayerTypeName = "";

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

void DissectResultBase::AppendSummery(QString field){
    if( this->summery.isEmpty() )
        this->summery.append(field);
    else
        this->summery.append(" " + field);
}

bool DissectResultBase::ContainProtocol(QString proName){
    if(this->protocolList.contains(proName))
        return true;
    else
        return false;
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

QString DissectResultBase::GetTopProtocolName(){
    return this->protocolList.last();
}

QString DissectResultBase::GetProtocolNameByIndex(qint32 index){
    if(index < this->GetProtocolListLength())
        return this->protocolList.at(index);
    else
        return "";
}

QString DissectResultBase::GetProtocolsInFrame(){
    QString protocolsInFrame = "";
    for(qint32 index = 0; index < this->protocolList.length(); index++){
        protocolsInFrame.append(this->protocolList.at(index));
        if( index != this->protocolList.length() - 1 )
            protocolsInFrame.append(":");
    }
    return protocolsInFrame;
}

QString DissectResultBase::GetNextProtocolName(QString currentProtocolName){
    if( this->protocolList.contains(currentProtocolName) ){
        qint32 index = this->protocolList.indexOf(currentProtocolName);
        if( index+ 1 < this->protocolList.length() )
            return this->protocolList.at(index + 1);
        else
            return "Non-Nextlayer or Non-Nextlayer-Dissector";
    }else{
        return "Non-Current-Protocol";
    }

}

const quint8* DissectResultBase::GetProtocolHeaderStartPtrByName(QString protocolName){
    if(this->protocolPositionHash.contains(protocolName))
        return this->data + this->protocolPositionHash.value(protocolName).start;
    else
        return NULL;
}

qint64 DissectResultBase::GetProtocolHeaderStartPositionByName(QString protocolName){
    if(this->protocolPositionHash.contains(protocolName))
        return this->protocolPositionHash.value(protocolName).start;
    else
        return -1;
}

const quint8* DissectResultBase::GetProtocolHeaderEndPtrByName(QString protocolName){
    if(this->protocolPositionHash.contains(protocolName))
        return this->data + this->protocolPositionHash.value(protocolName).end;
    else
        return NULL;
}

qint64 DissectResultBase::GetProtocolHeaderEndPositionByName(QString protocolName){
    if(this->protocolPositionHash.contains(protocolName))
        return this->protocolPositionHash.value(protocolName).end;
    else
        return -1;
}

QString DissectResultBase::GetSummery(){
    return this->summery;
}

/*保留字段的操作*/
//void DissectResultBase::ClearReserve(){
//    this->reserve_ptr.clear();
//    this->reserve_val.clear();
//}

//void DissectResultBase::ClearReserveVal(){
//    this->reserve_val.clear();
//}

//void DissectResultBase::ClearReservePtr(){
//    this->reserve_ptr.clear();
//}

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

//void DissectResultBase::RemoveAdditional(QString name){
//    this->reserve_ptr.remove(name);
//    this->reserve_val.remove(name);
//}

//void DissectResultBase::OrToAddition(QString name, qint64 val){
//    if( !this->reserve_val.contains(name) ){
//        this->reserve_val.insert(name,val);
//    }else{
//        (*this->reserve_val.find(name)) = this->reserve_val.value(name) | val;
//    }
//}

//void DissectResultBase::RemoveAdditionalVal(QString name){
//    this->reserve_val.remove(name);
//}

//void DissectResultBase::RemoveAdditionalPtr(QString name){
//    this->reserve_ptr.remove(name);
//}

qint64 DissectResultBase::GetAdditionalVal(QString name){
    if(!this->reserve_val.contains(name))
        return 0;
    return this->reserve_val.value(name);
}

void* DissectResultBase::GetAdditionalPtr(QString name, qint32 *len){
    if(!this->reserve_ptr.contains(name))
        return nullptr;
    if( len != nullptr )
        *len = this->reserve_ptr.value(name).len;
    return this->reserve_ptr.value(name).ptr;
}

//bool DissectResultBase::ReserveTcpContain(QString name){
//    return this->reserve_tcp.contains(name);
//}

//void DissectResultBase::AddAdditional(QString name, TcpInfo &tcpInfo){
//    this->reserve_tcp.insert(name,tcpInfo);
//}

//TcpInfo& DissectResultBase::GetAdditional(QString name){
//    return *this->reserve_tcp.find(name);
//}


void DissectResultBase::SetInterfaceInfo(qint32 interfaceId, QString interfaceName, qint32 linklayer, QString linklayerName){
    DissectResultBase::interfaceId = interfaceId;
    DissectResultBase::interfaceName.clear();
    DissectResultBase::interfaceName.append(interfaceName);
    DissectResultBase::linklayerType = linklayer;
    DissectResultBase::linklayerTypeName.clear();
    DissectResultBase::linklayerTypeName.append(linklayerName);
}

qint32 DissectResultBase::GetInterfaceId(){
    return DissectResultBase::interfaceId;
}

QString DissectResultBase::GetInterfaceName(){
    return DissectResultBase::interfaceName;
}

qint32 DissectResultBase::GetLinklayerType(){
    return DissectResultBase::linklayerType;
}

QString DissectResultBase::GetLinklayerTypeName(){
    return DissectResultBase::linklayerTypeName;
}

QHash<QString,quint64>* DissectResultBase::GetDissectorOptionPtr(){
    return DissectResultBase::dissectorOptions;
}

qint32 DissectResultBase::GetProtocolLength(QString name){
    if( this->protocolPositionHash.keys().contains(name) )
        if( this->GetTopProtocolName() != name)
            return this->protocolPositionHash.value(name).end - this->protocolPositionHash.value(name).start;
        else
            return this->GetPkthdr()->caplen - this->protocolPositionHash.value(name).start;
    else
        return 0;
}
