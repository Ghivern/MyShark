#include "stream.h"

Stream::Stream()
{
    this->maxStreamIndexPlusOne = 1;
}

qint64 Stream::Add(DissectResultBase *dissectResultBase,quint8 *srcAddr, quint8 *dstAddr, qint32 addr_size, quint8 *srcPort, quint8 *dstPort, qint32 port_size){
    if( dissectResultBase->GetIndex() == 0 && this->streamHash.count() > 0){
        this->ClearStream();
    }
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData((char*)srcAddr,addr_size);
    hash.addData((char*)dstAddr,addr_size);
    if(srcPort != NULL && dstPort != NULL){
        hash.addData((char*)srcPort,port_size);
        hash.addData((char*)dstPort,port_size);
    }
    QByteArray md5 = hash.result();
    if(this->streamHash.contains(md5)){
        (*this->streamHash.find(md5)).indexes.append(dissectResultBase->GetIndex());
        return this->streamHash.value(md5).streamIndexPlusOne;
    }else{
        hash.reset();
        hash.addData((char*)dstAddr,addr_size);
        hash.addData((char*)srcAddr,addr_size);
        if(srcPort != NULL && dstPort != NULL){
            hash.addData((char*)dstPort,port_size);
            hash.addData((char*)srcPort,port_size);
        }
        QByteArray conversedMd5 = hash.result();
        struct indexes_streamIndex new_index_streamIndex;
        new_index_streamIndex.indexes.append(dissectResultBase->GetIndex());
        new_index_streamIndex.srcAddr.append(Converter::ConvertQuint8ArrayToDecStr(srcAddr,addr_size,"."));
        new_index_streamIndex.srcPort = (srcPort == nullptr ? 0 : ntohs(*(quint16*)srcPort));
        new_index_streamIndex.dstAddr.append(Converter::ConvertQuint8ArrayToDecStr(dstAddr,addr_size,"."));
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

QString Stream::GetSourceAddress(qint64 streamIndexPlusOne){
    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).srcAddr;
}

quint16 Stream::GetSourcePort(qint64 streamIndexPlusOne){
    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).srcPort;
}

QString Stream::GetDestinationAddress(qint64 streamIndexPlusOne){
    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).dstAddr;
}

quint16 Stream::GetDestinationPort(qint64 streamIndexPlusOne){
    return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).dstPort;
}

void Stream::ClearStream(){
    this->md5Hash.clear();
    this->streamHash.clear();
    this->maxStreamIndexPlusOne = 1;
}
