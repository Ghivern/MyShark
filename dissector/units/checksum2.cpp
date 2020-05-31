#include "checksum2.h"

Checksum2::Checksum2(DissectResultBase *dissectResultBase,QString protocol){
    this->dissectResultBase = dissectResultBase;
    this->protocol = protocol;
}

Checksum2::~Checksum2(){

}

quint16 Checksum2::GetCalculateChecksum(){
    quint16 calculatedChecksum = 0;
    quint8* pseudoHeader = nullptr;
    if( this->protocol == "ipv4" ){
        Ipv4Info *ipv4Info = (Ipv4Info*)this->dissectResultBase->GetAdditionalPtr(IPV4_INFO_PTR);
        calculatedChecksum = this->getCalculateChecksum(ipv4Info->header
                                          ,ipv4Info->headerLen
                                          ,IPV4_CHECKSUM_POSITION
                                          ,nullptr
                                          ,0);
    }else if( this->protocol == "tcp" ){
        quint32 pseudoHeaderLen = 0;
        TcpInfo *tcpInfo = (TcpInfo*)this->dissectResultBase->GetAdditionalPtr(TCP_INFO_PTR);
        if( this->dissectResultBase->ContainProtocol("ipv4") )
            pseudoHeaderLen = IPV4_ADDR_LEN * 2 + ZERO_CHAR_LEN + PROTOCOL_NUMBER_LEN + LENGTH_LEN;
        else
            pseudoHeaderLen = 0;
        pseudoHeader = this->producePseudoHeader();
        calculatedChecksum = this->getCalculateChecksum(
                    this->dissectResultBase->GetProtocolHeaderStartPtrByName("tcp")
                                          ,tcpInfo->totalLen
                                          ,TCP_CHECKSUM_POSITION
                                          ,pseudoHeader
                                          ,pseudoHeaderLen);
    }else if( this->protocol == "udp" ){
        quint32 pseudoHeaderLen = 0;
        UdpInfo *udpInfo = (UdpInfo*)this->dissectResultBase->GetAdditionalPtr(UDP_INFO_PTR);
        if( this->dissectResultBase->ContainProtocol("ipv4") )
            pseudoHeaderLen = IPV4_ADDR_LEN * 2 + ZERO_CHAR_LEN + PROTOCOL_NUMBER_LEN + LENGTH_LEN;
        else
            pseudoHeaderLen = 0;
        qDebug() << "开始调用producePseudoHeader";
        pseudoHeader = this->producePseudoHeader();
        qDebug() << "开始调用getCalculateChecksum";
        calculatedChecksum = this->getCalculateChecksum(
                    this->dissectResultBase->GetProtocolHeaderStartPtrByName("udp")
                    ,udpInfo->totalLen
                    ,UDP_CHECKSUM_POSITION
                    ,pseudoHeader
                    ,pseudoHeaderLen
                    );
    }

    if( pseudoHeader != nullptr )
        free(pseudoHeader);
    return calculatedChecksum;
}

QString Checksum2::GetCalculateChecksumStr(){
    quint16 calculatedChecksum = this->GetCalculateChecksum();
    return Converter::ConvertQuint8ArrayToHexStr((quint8*)&calculatedChecksum,2);
}

/* Pseudo Header for Ipv4 Format as follow
* --------------|---------------|---------------|----------------
* Source Address                                                |
* ---------------------------------------------------------------
* Destination Address                                           |
* ---------------------------------------------------------------
* Zero          | Protocol      |  Up-Layer Packet Length       |
* ---------------------------------------------------------------
*/

quint8* Checksum2::producePseudoHeader(){
    quint8 *pseudoHeader = nullptr;
    quint16 temp;
    if( this->protocol == "tcp" ){
        if( this->dissectResultBase->ContainProtocol("ipv4") ){
            Copy copy(IPV4_ADDR_LEN * 2 + ZERO_CHAR_LEN + PROTOCOL_NUMBER_LEN + LENGTH_LEN);
            Ipv4Info *ipv4Info = (Ipv4Info*)dissectResultBase->GetAdditionalPtr(IPV4_INFO_PTR);
            TcpInfo *tcpInfo = (TcpInfo*)dissectResultBase->GetAdditionalPtr(TCP_INFO_PTR);
            temp = htons(tcpInfo->totalLen);
            pseudoHeader =
                    copy.Append(ipv4Info->src,IPV4_ADDR_LEN)
                    .Append(ipv4Info->dst,IPV4_ADDR_LEN)
                    .Append(&ZERO,ZERO_CHAR_LEN)
                    .Append(&TCP_PROTOCOL_NUM,PROTOCOL_NUMBER_LEN)
                    .Append((quint8*)&temp,LENGTH_LEN)
                    .GetData();

//            pseudoHeader = new quint8[IPV4_ADDR_LEN * 2 + ZERO_CHAR_LEN + PROTOCOL_NUMBER_LEN + LENGTH_LEN ];
//            Ipv4Info *ipv4Info = (Ipv4Info*)dissectResultBase->GetAdditionalPtr(IPV4_INFO_PTR);
//            TcpInfo *tcpInfo = (TcpInfo*)dissectResultBase->GetAdditionalPtr(TCP_INFO_PTR);
//            memcpy(pseudoHeader,ipv4Info->src,IPV4_ADDR_LEN);
//            memcpy(pseudoHeader + IPV4_ADDR_LEN,ipv4Info->dst,IPV4_ADDR_LEN);
//            memcpy(pseudoHeader + IPV4_ADDR_LEN * 2,&TCP_PROTOCOL_NUM,1);
//            memcpy(pseudoHeader + IPV4_ADDR_LEN * 2 + PROTOCOL_NUMBER_LEN,&ZERO,1);
//            temp = htons(tcpInfo->totalLen);
//            memcpy(pseudoHeader + IPV4_ADDR_LEN * 2 + PROTOCOL_NUMBER_LEN + ZERO_CHAR_LEN,(quint8*)&temp,2);
        }
    }else if( this->protocol == "udp" ){ //this->protocol == "udp"
        Copy copy(IPV4_ADDR_LEN * 2 + ZERO_CHAR_LEN + PROTOCOL_NUMBER_LEN + LENGTH_LEN);
        Ipv4Info *ipv4Info = (Ipv4Info*)dissectResultBase->GetAdditionalPtr(IPV4_INFO_PTR);
        UdpInfo *udpInfo = (UdpInfo*)dissectResultBase->GetAdditionalPtr(UDP_INFO_PTR);
        temp = htons(udpInfo->totalLen);
        pseudoHeader =
                copy.Append(ipv4Info->src,IPV4_ADDR_LEN)
                .Append(ipv4Info->dst,IPV4_ADDR_LEN)
                .Append(&ZERO,ZERO_CHAR_LEN)
                .Append(&UDP_PROTOCOL_NUM,PROTOCOL_NUMBER_LEN)
                .Append((quint8*)&temp,LENGTH_LEN)
                .GetData();
    }
    return pseudoHeader;
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
                this->getInverseSum(&checksum,start[index]);
        }
    }

    start = (quint16*)data;
    for(index = 0; index < dataLen/2; index++){
        if(index != checksumPosition){
            this->getInverseSum(&checksum,start[index]);
        }
    }

    return ~checksum;
}
