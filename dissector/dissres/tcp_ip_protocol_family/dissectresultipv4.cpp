#include "dissectresultipv4.h"

using namespace tcp_ip_protocol_family;

Stream DissectResultIpv4::stream;

DissectResultIpv4::DissectResultIpv4(DissectResultBase *dissectResultBase){
    this->protocol_family_transport_layer = NULL;
    this->dissectResultBase = dissectResultBase;
    dissectResultBase->PushToProtocolList("ipv4",NETWORKLAYER_IPV4_FIELD_LENGTH_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ipv4");
    if(this->header != NULL){
        dissectResultBase->UpdateProtocolList("ipv4",this->GetHeaderLength() * 4);
        dissectResultBase->UpdateProtocolHeaderLengthCount(this->GetHeaderLength() * 4);
        this->streamIndexPlusOne = DissectResultIpv4::stream.Add(dissectResultBase,header->srcaddr,header->dstaddr,NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR);

        this->AddNextLayer(dissectResultBase,(NETWORKLAYER_IPV4_PROTOCOL_TYPE)*header->type);
    }

    if(dissectResultBase->GetPkthdr()->caplen - dissectResultBase->GetAdditionalVal("linklayer_have_fcs") - this->GetTotalLength() < 4)
        *((bool*)dissectResultBase->GetAdditionalPtr("linklayer_have_fcs")) = false;
    dissectResultBase->RemoveAdditional("linklayer_have_fcs");
}

void DissectResultIpv4::AddNextLayer(DissectResultBase *dissectResultBase, NETWORKLAYER_IPV4_PROTOCOL_TYPE type){
    switch (type) {
        case NETWORKLAYER_IPV4_TYPE_TCP:
        {
        this->dissectResultBase->AddAdditional(IP_PERSUDO_HEADER_PTR,this->producePseudoHeader(NETWORKLAYER_IPV4_TYPE_TCP));
        dissectResultBase->AddAdditional(TCP_TOTAL_LEN,this->GetTotalLength() - this->GetHeaderLength() * 4);
        dissectResultBase->AddAdditional(IP_SOURCE_ADDRESS_PTR,header->srcaddr);
        dissectResultBase->AddAdditional(IP_DESTINATION_ADDRESS_PTR,header->dstaddr);
        dissectResultBase->AddAdditional(IP_ADDRESS_LENGTH,NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR);
        this->protocol_family_transport_layer = (void*)(new DissectResultTcp(dissectResultBase));
        break;
        }
    case NETWORKLAYER_IPV4_TYPE_UDP:
        this->dissectResultBase->AddAdditional(IP_PERSUDO_HEADER_PTR,this->producePseudoHeader(NETWORKLAYER_IPV4_TYPE_UDP));
        this->protocol_family_transport_layer = (void*)(new DissectResultUdp(dissectResultBase));
        break;
    }
}

void* DissectResultIpv4::GetNextLayer(){
    return this->protocol_family_transport_layer;
}


/*
 *  以下均是解析Ipv4首部字段的方法,调用前提是，this->header已经被正确赋值
 *
 *0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *Version|Hdr Len|   DS          |     total length              |
 *   identification              |flag | offset                  |
 *   ttl         | protocol      |    header checksum            |
 *                src address                                    |
 *                dst address                                    |
 * options            .  padding                                 |
 *                    data                                       |
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * 他
 * Hdr Len :单位是4字节
 * DS: Differntiated Service   RFC 2474 3168 3260
 * DS: bit0~1 ECT ;bit2~7 DSCP
 * offset:  片偏移
 * ttl : time to live
 * 考虑到Ipv4的首部拓展很少使用，所以暂时不做处理
 */
/*Version-HeaderLength*/
const quint8* DissectResultIpv4::GetVersionHeaderLengthPtr(){
    return this->header->version_hdrLen;
}

quint8 DissectResultIpv4::GetVersion(){
    return (*this->header->version_hdrLen & 0xf0) >> 4;
}

quint8 DissectResultIpv4::GetHeaderLength(){
    return *this->header->version_hdrLen & 0x0f;
}

/*Differentiated Service*/
const quint8* DissectResultIpv4::GetDifferentiatedServicePrt(){
    return this->header->DS;
}

quint8 DissectResultIpv4::GetDSField_ECN(){
    return *this->header->DS & 0x03;
}

QString DissectResultIpv4::GetDSFField_ECN_short_meaning(){
    return ecn_short_vals.value(this->GetDSField_ECN());
}

QString DissectResultIpv4::GetDSField_ECN_meanning(){
    return ecn_vals.value(this->GetDSField_ECN());
}

quint8 DissectResultIpv4::GetDSField_DSCP(){
    return (*this->header->DS & 0xfc) >> 2;
}

QString DissectResultIpv4::GetDSField_DSCP_short_meaning(){
    return dscp_short_vals.value(this->GetDSField_DSCP());
}

QString DissectResultIpv4::GetDSField_DSCP_meanning(){
    return dscp_vals.value(this->GetDSField_DSCP());
}

QString DissectResultIpv4::GetDifferentiatedServiceStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->DS,NETWORKLAYER_IPV4_FIELD_LENGTH_DS);
}


/*Total Length*/
const quint8* DissectResultIpv4::GetTotalLengthPrt(){
    return this->header->totalLen;
}

quint16 DissectResultIpv4::GetTotalLength(){
    return ntohs(*(quint16*)this->header->totalLen);
}

/*Identification*/
const quint8* DissectResultIpv4::GetIdentificationPtr(){
    return this->header->ident;
}

quint16 DissectResultIpv4::GetIdentification(){
    return ntohs(*(quint16*)this->header->ident);
}

QString DissectResultIpv4::GetIdentificationStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->ident,NETWORKLAYER_IPV4_FIELD_LENGTH_IDENT);
}

/*Flag-Offset
 * Reseveed|DF|MF|Fragment Offset(13)
 */
const quint8* DissectResultIpv4::GetFlagOffsetPtr(){
    return this->header->flag_offset;
}

QString DissectResultIpv4::GetFlagOffsetStr(){
    Converter converter(this->header->flag_offset,NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET);
    return converter.ConvertQuint8ArrayToHexStr();
}

bool DissectResultIpv4::Reserve(){
    if(Bit::GetBitFromOctetPtr(this->header->flag_offset,7))
        return true;
    return false;
}

QString DissectResultIpv4::Reserve_meaning(){
    if(this->Reserve())
        return "Set";
    return "Not Set";
}

bool DissectResultIpv4::DF(){
    if(Bit::GetBitFromOctetPtr(this->header->flag_offset,6))
        return true;
    return false;
}

QString DissectResultIpv4::DF_meaning(){
    if(this->DF())
        return "Set";
    return "Not Set";
}

bool DissectResultIpv4::MF(){
    if(Bit::GetBitFromOctetPtr(this->header->flag_offset,5))
        return true;
    return false;
}

QString DissectResultIpv4::MF_meaning(){
    if(this->MF())
        return "Set";
    return "Not Set";
}

quint16 DissectResultIpv4::GetFragmentOffset(){
    /*|***X|XXXX|YYYY|YYYY| -- > |***Y|YYYY|YYYX|XXXX| */
    quint16 res = *this->header->flag_offset << 3;
    res <<= 8;
    res |= *(this->header->flag_offset + 1);
    return ntohs(res) >> 3;
}


/*Time To Live*/
const quint8* DissectResultIpv4::GetTimeToLivePtr(){
    return this->header->ttl;
}

quint8 DissectResultIpv4::GetTimeToLive(){
    return *this->header->ttl;
}

/*Type*/
const quint8* DissectResultIpv4::GetTypePtr(){
    return this->header->type;
}

quint8 DissectResultIpv4::GetType(){
    return *this->header->type;
}

QString DissectResultIpv4::GetTypeName(){
    switch (*this->header->type) {
    case NETWORKLAYER_IPV4_TYPE_TCP:
        return "tcp";
    case NETWORKLAYER_IPV4_TYPE_UDP:
        return "udp";
    default:
        return "networklayer_ipv4_undeal_type";
    }
}

/*Checksum*/
const quint8* DissectResultIpv4::GetChecksumPtr(){
    return this->header->headerchecksum;
}

QString DissectResultIpv4::GetChecksumStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->headerchecksum,NETWORKLAYER_IPV4_FIELD_LENGTH_HEADERCHECKSUM);
}

/*Source Address*/
const quint8* DissectResultIpv4::GetSourceAddressPtr(){
    return this->header->srcaddr;
}

QString DissectResultIpv4::GetSourceAddressStr(){
    return Converter::ConvertQuint8ArrayToDecStr(this->header->srcaddr,NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR,".","");
}

/*Destination Address*/
const quint8* DissectResultIpv4::GetDestinationAddressPtr(){
    return this->header->dstaddr;
}

QString DissectResultIpv4::GetDestinationAddressStr(){
    return Converter::ConvertQuint8ArrayToDecStr(this->header->dstaddr,NETWORKLAYER_IPV4_FIELD_LENGTH_DSTADDR,".","");
}

/*Private*/
quint8* DissectResultIpv4::producePseudoHeader(NETWORKLAYER_IPV4_PROTOCOL_TYPE type){
    quint8 *pseudoHeader = (quint8*)malloc(this->pseudoHeaderLen);
    memcpy(pseudoHeader,this->header->srcaddr,NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2);
    pseudoHeader[NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2] = 0;
    pseudoHeader[NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2 + 1] = type;
    quint16 total_len = this->GetTotalLength() - this->GetHeaderLength() * 4;
    *(quint16*)(pseudoHeader + NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2 + 2) = htons(total_len);
    return pseudoHeader;
}
