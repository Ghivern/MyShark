#include "stream.h"

Stream::Stream()
{
    this->maxStreamIndexPlusOne = 1;
}

qint64 Stream::Add(DissectResultBase *dissectResultBase,quint8 *srcAddr, quint8 *dstAddr, qint32 addr_size, quint8 *srcPort, quint8 *dstPort, qint32 port_size){
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
        if(this->streamHash.contains(conversedMd5))
            new_index_streamIndex.streamIndexPlusOne = -this->streamHash.value(conversedMd5).streamIndexPlusOne;
        else
            new_index_streamIndex.streamIndexPlusOne = this->maxStreamIndexPlusOne++;
        this->streamHash.insert(md5,new_index_streamIndex);
        this->md5Hash.insert(new_index_streamIndex.streamIndexPlusOne,md5);
        return new_index_streamIndex.streamIndexPlusOne;
    }
}

QList<quint64> Stream::GetPacketsIndexListByStream(qint64 streamIndexPlusOne){
    if(!this->md5Hash.contains(streamIndexPlusOne)){
        QList<quint64> emptyList;
        return emptyList;
    }else{
        return this->streamHash.value(this->md5Hash.value(streamIndexPlusOne)).indexes;
    }
}
