#include "checksum2.h"

Checksum2::Checksum2(DissectResultBase *dissectResultBase,QString protocol){
    this->dissectResultBase = dissectResultBase;
    this->protocol = protocol;
}

Checksum2::~Checksum2(){

}

quint16 Checksum2::GetCalculateChecksum(){
    if( this->protocol == "ipv4" ){
        Ipv4Info *ipv4Info = (Ipv4Info*)this->dissectResultBase->GetAdditionalPtr(IPV4_INFO_PTR);
        return this->getCalculateChecksum(ipv4Info->header
                                          ,ipv4Info->headerLen
                                          ,IPV4_CHECKSUM_POSITION
                                          ,nullptr
                                          ,0);
    }else{
        return 0;
    }
}

QString Checksum2::GetCalculateChecksumStr(){
    quint16 calculatedChecksum = this->GetCalculateChecksum();
    return Converter::ConvertQuint8ArrayToHexStr((quint8*)&calculatedChecksum,2);
}

void Checksum2::getInverseSum(quint16 *sum,quint16 num){
    quint32 res = *sum + num;
    if(res > 0x0000ffff)
        *sum =  (quint16)(res - 0x00010000 + 1);
    else
        *sum = (quint16)(res);
}

quint16 Checksum2::getCalculateChecksum(const quint8 *data,qint32 dataLen,qint32 checksumPosition,quint8 *pseudoHeaderPtr,qint32 pseudoHeaderLength){
    const quint16 *start = nullptr;
    qint32 index;
    quint16 checksum = 0;

    if(pseudoHeaderPtr != nullptr){
        start = (quint16*)pseudoHeaderPtr;
        for(index = 0; index < pseudoHeaderLength/2; index++){
            if(index != checksumPosition)
                this->getInverseSum(&checksum,start[index]);
        }
    }

    start = (quint16*)data;;
    for(index = 0; index < dataLen/2; index++){
        if(index != checksumPosition){
            this->getInverseSum(&checksum,start[index]);
        }
    }

    return ~checksum;
}
