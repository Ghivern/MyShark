#include "checksum.h"
#include "../dissectors/dissectoreth/eth_header.h"

CheckSum::CheckSum(DissResEth *dissResEth,QString proName,quint16 length, bool withPseudoHeader){
    quint8 proNumber = 0;
    this->done = false;
    this->checksum = 0;
    this->data = dissResEth->GetData() + dissResEth->GetProStart(proName);
    this->dataLength = length;

    if(withPseudoHeader){
        proNumber = (proName == "tcp")?6:17;
        this->GetPseudoHeader(dissResEth,proNumber,length);
    }else{
        this->pseudoHeaderPtr = NULL;
        this->pseudoHeaderLength = 0;
    }

    if( proName == "tcp" )
        this->checksum_position = TCP_CHECKSUM_POSITION;
    else if( proName == "udp" )
        this->checksum_position = UDP_CHECKSUM_POSITION;
    else
        this->checksum_position = IPV4_CHECKSUM_POSITION;
}

quint16 CheckSum::GetChecksum(){
    quint16 *start = NULL;
    qint32 index;
    if(this->done){
        return this->checksum;
    }

    if(this->pseudoHeaderPtr != NULL){
        start = (quint16*)this->pseudoHeaderPtr;
        for(index = 0; index < this->pseudoHeaderLength/2; index++){
            if(index != this->checksum_position)
                this->GetInverseSum(&this->checksum,ntohs(start[index]));
        }
    }

    start = (quint16*)this->data;;
    for(index = 0; index < this->dataLength/2; index++){
        if(index != this->checksum_position)
            this->GetInverseSum(&this->checksum,ntohs(start[index]));
    }

    this->done = true;
    return this->checksum;
}

QString CheckSum::GetStrChecksum(){
    quint8 *checksumPtr = (quint8*)&this->checksum;
    if(!this->done)
        this->GetChecksum();
    return QString::asprintf("0x%02x%02x",checksumPtr[1],checksumPtr[0]);
}

quint8* CheckSum::GetPseudoHeader(DissResEth *dissResEth,quint8 proNumber,quint16 length){
    quint64 addr[4];
    qint32 addrLen = (dissResEth->ipVersion() == 4)?IPV4_ADDR_LEN:IPV6_ADDR_LEN;
    quint8 *tptr, *pseudoHeader = NULL;
    quint16 netLength;
    this->pseudoHeaderLength = addrLen*2 + ZERO_CHAR + PROTOCOL_NUMBER + LENGTH;
    pseudoHeader = (quint8*)malloc(this->pseudoHeaderLength);

    if(dissResEth->ipVersion() == 4){
        quint32 *addr32 = (quint32*)addr;
        addr32[0] = dissResEth->GetIpSrcAddressNetworkOrder();
        addr32[1] = dissResEth->GetIpDstAddressNetworkOrder();
    }else{
        dissResEth->GetIpv6SrcAddressNetworkOrder(addr+1,addr);
        dissResEth->GetIpv6DstAddressNetworkOrder(addr+3,addr+2);
    }
    memcpy(pseudoHeader,addr,addrLen * 2);
    tptr = pseudoHeader + addrLen * 2;

    addr[0] = 0;
    memcpy(tptr,addr,ZERO_CHAR);
    tptr += ZERO_CHAR;

    memcpy(tptr,&proNumber,PROTOCOL_NUMBER);
    tptr += PROTOCOL_NUMBER;

    netLength = htons(length);
    memcpy(tptr,&netLength,LENGTH);

    return pseudoHeader;
}

void CheckSum::GetInverseSum(quint16 *sum, quint16 num){
    quint32 res = (quint32)*sum + (quint32)num;
    if(res > 0x0000ffff)
        *sum =  (quint16)(res - 0x00010000 + 1);
    else
        *sum = (quint16)(res);
}

