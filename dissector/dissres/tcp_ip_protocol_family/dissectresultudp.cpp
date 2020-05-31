#include "dissectresultudp.h"

using namespace tcp_ip_protocol_family;

Stream DissectResultUdp::stream;

DissectResultUdp::DissectResultUdp(DissectResultBase *dissectResultBase,void *reserves):DissectResult(dissectResultBase)
{
    quint64 options = DissectResultBase::GetDissectorOptionPtr()->value("udp");
    UdpInfo *udpInfo = new UdpInfo;

    dissectResultBase->PushToProtocolList("udp",TRANSPORTLAYER_UDP_FIELD_LENGTH_TEMP_HEADER_LENGTH);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("udp");

    if( this->GetDissectResultBase()->ContainProtocol("ipv4") ){
        Ipv4Info *ipv4Info = (Ipv4Info*)this->GetDissectResultBase()->GetAdditionalPtr(IPV4_INFO_PTR);
        this->SetStremIndexPlusOne(
                    stream.Add(dissectResultBase
                               ,ipv4Info->src
                               ,ipv4Info->dst
                               ,ipv4Info->addrLen
                               ,this->GetSourcePortPtr()
                               ,this->GetDestinationPortPtr()
                               ,TRANSPORTLAYER_UDP_FIELD_LENGTH_SOURCE_PORT)
                    );
    }else{
        this->SetStremIndexPlusOne(0);
    }

    udpInfo->totalLen = this->GetTotalLen();
    this->GetDissectResultBase()->AddAdditional(UDP_INFO_PTR,udpInfo);

    if( options & UDP_VALIDATE_CHECKSUM ){
        //计算校验和
        qDebug() << "创建Udp的检验和";
        Checksum2 checksum(this->GetDissectResultBase(),"udp");
        qDebug() << "创建成功";
        this->calculatedChecksumStr = checksum.GetCalculateChecksumStr();
        qDebug() << "GetCalculateChecksunStr() 成功";
        if( this->calculatedChecksumStr != this->GetChecksumStr() )
            udpInfo->badChecksum = true;
    }

    if( header != nullptr )
        this->addNextLayer(dissectResultBase,reserves);
}

quint8* DissectResultUdp::GetSourcePortPtr(){
    return this->header->srcPort;
}

quint8* DissectResultUdp::GetDestinationPortPtr(){
    return this->header->dstPort;
}

quint16 DissectResultUdp::GetSourcePort(){
    return ntohs(*(quint16*)this->header->srcPort);
}

quint16 DissectResultUdp::GetDestinationPort(){
    return ntohs(*(quint16*)this->header->dstPort);
}

quint16 DissectResultUdp::GetLength(){
    return ntohs(*(quint16*)this->header->length);
}

qint32 DissectResultUdp::GetTotalLen(){
    return this->GetLength();
}

quint8* DissectResultUdp::GetChecksumPtr(){
    return this->header->checksum;
}

QString DissectResultUdp::GetChecksumStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->checksum,TRANSPORTLAYER_UDP_FIELD_LENGTH_LENGTH);
}

QString DissectResultUdp::GetCalculatedChecksum(){
    return this->calculatedChecksumStr;
}

Stream& DissectResultUdp::GetStreamRecorder(){
    return DissectResultUdp::stream;
}

void DissectResultUdp::addNextLayer(DissectResultBase *dissectResultBase, void *reserves){
    Q_UNUSED(dissectResultBase)
    Q_UNUSED(reserves)
}
