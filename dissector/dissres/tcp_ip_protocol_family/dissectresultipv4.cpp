#include "dissectresultipv4.h"

using namespace tcp_ip_protocol_family;

Stream DissectResultIpv4::stream;

DissectResultIpv4::DissectResultIpv4(DissectResultBase *dissectResultBase,void *reserves)
        :DissectResult(dissectResultBase)
{
    Q_UNUSED(reserves)
    quint64 options = DissectResultBase::GetDissectorOptionPtr()->value("ipv4");
    dissectResultBase->PushToProtocolList("ipv4",NETWORKLAYER_IPV4_FIELD_LENGTH_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ipv4");

    if(this->header != NULL){
        dissectResultBase->UpdateProtocolList("ipv4",this->GetHeaderLength() * 4);
        dissectResultBase->UpdateProtocolHeaderLengthCount(this->GetHeaderLength() * 4);

        /*填充Ipv4Info*/
        Ipv4Info *ipv4Info = new Ipv4Info;
        ipv4Info->header = (quint8*)this->header;
        ipv4Info->headerLen = this->GetHeaderLength() * 4;
        ipv4Info->payloadLen = this->GetTotalLength() - this->GetHeaderLength() * 4;
        ipv4Info->src = header->srcaddr;
        ipv4Info->dst = header->dstaddr;
        ipv4Info->addrLen = NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR;
        dissectResultBase->AddAdditional(IPV4_INFO_PTR,ipv4Info);

        if( options & IPV4_VALIDATE_CHECKSUM ){
            Checksum2 checksum2(dissectResultBase,"ipv4");
            this->calculatedChecksum = checksum2.GetCalculateChecksumStr();
            if( this->calculatedChecksum != this->GetChecksumStr() )
                ipv4Info->status |= IPV4_A_BAD_CHECKSUM;
        }else{
            this->calculatedChecksum = "";
        }

        this->SetStremIndexPlusOne( DissectResultIpv4::stream
                                    .Add(dissectResultBase,header->srcaddr,header->dstaddr
                                    ,NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR));

        this->addNextLayer(dissectResultBase,(NETWORKLAYER_IPV4_PROTOCOL_TYPE)*header->type,reserves);
    }

//    if(dissectResultBase->GetPkthdr()->caplen - dissectResultBase->GetAdditionalVal("linklayer_have_fcs") - this->GetTotalLength() < 4)
//        *((bool*)dissectResultBase->GetAdditionalPtr("linklayer_have_fcs")) = false;
//    dissectResultBase->RemoveAdditional("linklayer_have_fcs");
}


/*Version-HeaderLength*/
const quint8* DissectResultIpv4::GetVersionHeaderLengthPtr(){
    return this->header->version_hdrLen;
}

quint8 DissectResultIpv4::GetVersion(){
    return (*this->header->version_hdrLen & 0xf0) >> 4;
}

QString DissectResultIpv4::GetVersionDotStr(){
    return QString("%1%2%3%4 ....")
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,7))
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,6))
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,5))
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,4));
}

quint8 DissectResultIpv4::GetHeaderLength(){
    return *this->header->version_hdrLen & 0x0f;
}

QString DissectResultIpv4::GetHeaderLengthDotStr(){
    return QString(".... %1%2%3%4")
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,3))
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,2))
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,1))
            .arg(Bit::GetBitFromOctetPtr(this->header->version_hdrLen,0));
}

/*Differentiated Service*/
const quint8* DissectResultIpv4::GetDifferentiatedServicePrt(){
    return this->header->DS;
}

quint8 DissectResultIpv4::GetDSField_ECN(){
    return *this->header->DS & 0x03;
}

QString DissectResultIpv4::GetDSField_ECN_DotStr(){
    return Bit::GetDotStrFormOctetPtr(this->header->DS,0,1);
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

QString DissectResultIpv4::GetDSField_DSCP_DotStr(){
    return Bit::GetDotStrFormOctetPtr(this->header->DS,2,7);
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

QString DissectResultIpv4::GetReserveDotStr(){
    return Bit::GetDotStrFormOctetPtr(this->header->flag_offset,7,7).append(" .... ....");
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

QString DissectResultIpv4::GetDFDotStr(){
    return Bit::GetDotStrFormOctetPtr(this->header->flag_offset,6,6).append(" .... ....");
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

QString DissectResultIpv4::GetMFDotStr(){
    return Bit::GetDotStrFormOctetPtr(this->header->flag_offset,5,5).append(" .... ....");
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

QString DissectResultIpv4::GetFragmentOffsetDotStr(){
    return Bit::GetDotStrFormOctetPtr(this->header->flag_offset,0,4)
            + " "
            + Bit::GetDotStrFormOctetPtr(this->header->flag_offset + 1,0,7);
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

QString DissectResultIpv4::GetCalculatedChecksumStr(){
    return this->calculatedChecksum;
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
void DissectResultIpv4::addNextLayer(DissectResultBase *dissectResultBase
                                     , NETWORKLAYER_IPV4_PROTOCOL_TYPE type
                                     ,void *reserves){
    switch (type) {
    case NETWORKLAYER_IPV4_TYPE_TCP:
    {
        this->SetNextLayer( (void*)(new DissectResultTcp(dissectResultBase,reserves)));
        break;
    }
    case NETWORKLAYER_IPV4_TYPE_UDP:
    {
        this->SetNextLayer( (void*)(new DissectResultUdp(dissectResultBase,reserves)));
        break;
    }
    default:
    {
        this->GetDissectResultBase()->SetSummery(QString("未处理网络层类型: %1")
                                            .arg(this->GetType())
                                            );
        this->SetNextLayer();
    }
    }
}

//quint8* DissectResultIpv4::producePseudoHeader(NETWORKLAYER_IPV4_PROTOCOL_TYPE type){
//    quint8 *pseudoHeader = (quint8*)malloc(this->pseudoHeaderLen);
//    memcpy(pseudoHeader,this->header->srcaddr,NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2);
//    pseudoHeader[NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2] = 0;
//    pseudoHeader[NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2 + 1] = type;
//    quint16 total_len = this->GetTotalLength() - this->GetHeaderLength() * 4;
//    *(quint16*)(pseudoHeader + NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR*2 + 2) = htons(total_len);
//    return pseudoHeader;
//}
