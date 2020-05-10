#include "stream.h"

Stream::Stream(qint32 addrSize,qint32 portSize)
{
    this->maxStreamIndexPlusOne = 1;
    this->addrSize = addrSize;
    this->portSize = portSize;
}

qint64 Stream::Add(DissectResultBase *dissectResultBase, quint8 *srcAddr, quint8 *dstAddr, qint32 addr_size, quint8 *srcPort, quint8 *dstPort, qint32 port_size){
    this->addrSize = addr_size;
    this->portSize = port_size;
    return this->Add(dissectResultBase,srcAddr,dstAddr,srcPort,dstPort);
}

qint64 Stream::Add(DissectResultBase *dissectResultBase,quint8 *srcAddr, quint8 *dstAddr, quint8 *srcPort, quint8 *dstPort){
    if( dissectResultBase->GetIndex() == 0 && this->streamHash.count() > 0){
        this->ClearStream();
    }
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData((char*)srcAddr,this->addrSize);
    hash.addData((char*)dstAddr,this->addrSize);
    if(srcPort != nullptr && dstPort != nullptr && this->portSize != 0){
        hash.addData((char*)srcPort,this->portSize);
        hash.addData((char*)dstPort,this->portSize);
    }
    QByteArray md5 = hash.result();
    if(this->streamHash.contains(md5)){
        (*this->streamHash.find(md5)).indexes.append(dissectResultBase->GetIndex());
        return this->streamHash.value(md5).streamIndexPlusOne;
    }else{
        hash.reset();
        hash.addData((char*)dstAddr,this->addrSize);
        hash.addData((char*)srcAddr,this->addrSize);
        if(srcPort != nullptr && dstPort != nullptr && this->portSize != 0){
            hash.addData((char*)dstPort,this->portSize);
            hash.addData((char*)srcPort,this->portSize);
        }
        QByteArray conversedMd5 = hash.result();
        struct indexes_streamIndex new_index_streamIndex;
        new_index_streamIndex.indexes.append(dissectResultBase->GetIndex());
//        if( this->addrSize == 6 ){
//            new_index_streamIndex.srcAddr.append(Converter::ConvertQuint8ArrayToHexStr(srcAddr,this->addrSize,":",""));
//            new_index_streamIndex.dstAddr.append(Converter::ConvertQuint8ArrayToHexStr(dstAddr,this->addrSize,":",""));
//        }else{
//            new_index_streamIndex.srcAddr.append(Converter::ConvertQuint8ArrayToDecStr(srcAddr,this->addrSize,"."));
//            new_index_streamIndex.dstAddr.append(Converter::ConvertQuint8ArrayToDecStr(dstAddr,this->addrSize,"."));
//        }
        new_index_streamIndex.srcPort = (srcPort == nullptr ? 0 : ntohs(*(quint16*)srcPort));
        new_index_streamIndex.dstPort = (dstPort == nullptr ? 0 : ntohs(*(quint16*)dstPort));
        if(this->streamHash.contains(conversedMd5))
            new_index_streamIndex.streamIndexPlusOne = -this->streamHash.value(conversedMd5).streamIndexPlusOne;
        else
            new_index_streamIndex.streamIndexPlusOne = this->maxStreamIndexPlusOne++;
        this->streamHash.insert(md5,new_index_streamIndex);
        this->md5Hash.insert(new_index_streamIndex.streamIndexPlusOne,md5);
        return new_index_streamIndex.streamIndexPlusOne;
    }
}

qint32 Stream::GetStreamCount(){
    return qCeil(this->md5Hash.keys().length() * 1.0 / 2);
}

bool Stream::Contain(qint64 streamIndexPlusOne){
    return this->md5Hash.contains(streamIndexPlusOne);
}

QList<quint64> Stream::GetPacketsIndexListByStream(qint64 streamIndexPlusOne){
    if(!this->md5Hash.contains(streamIndexPlusOne)){
        QList<quint64> emptyList;
        return emptyList;
    }else{
        return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).indexes;
    }
}

//QString Stream::GetSourceAddress(qint64 streamIndexPlusOne){
//    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).srcAddr;
//}

quint16 Stream::GetSourcePort(qint64 streamIndexPlusOne){
    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).srcPort;
}

//QString Stream::GetDestinationAddress(qint64 streamIndexPlusOne){
//    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).dstAddr;
//}

quint16 Stream::GetDestinationPort(qint64 streamIndexPlusOne){
    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).dstPort;
}

void Stream::ClearStream(){
    this->md5Hash.clear();
    this->streamHash.clear();
    this->maxStreamIndexPlusOne = 1;
}

bool Stream::HavePort(){
    if( this->portSize == 0 )
        return false;
    return true;
}

