#include "dissectresultlinklayer.h"
#include <QDebug>

using namespace tcp_ip_protocol_family;

//const QString DissectResultLinkLayer::relative_dir = "../others/";
const QString DissectResultLinkLayer::relative_dir = ":/ethernet/";
const QString DissectResultLinkLayer::ethernet_address_file_path = relative_dir + "ethernet-addresses";
const QString DissectResultLinkLayer::ethernet_well_know_address_file_path = relative_dir + "ethernet-well-know-addresses";

QHash<QString,QString> DissectResultLinkLayer::ethernet_address_modify_hash;
QFile DissectResultLinkLayer::file_ethernet_address(ethernet_address_file_path);
QFile DissectResultLinkLayer::file_ethernet_well_know_address(ethernet_well_know_address_file_path);

Stream DissectResultLinkLayer::stream;

DissectResultLinkLayer::DissectResultLinkLayer(DissectResultBase *dissectResultBase,void *reserves):DissectResult(dissectResultBase)
{
    Q_UNUSED(reserves)
    this->haveFCS = true;
    //this->protocol_family_network_layer = NULL;
    //this->dissectResultBase = dissectResultBase;
    //this->SetDissectResultBase(dissectResultBase);
    dissectResultBase->PushToProtocolList("ether",LINKLAYER_FIELD_LENGTH::LINKLAYER_FIELD_LENGTH_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ether");
    dissectResultBase->UpdateProtocolHeaderLengthCount(LINKLAYER_FIELD_LENGTH_TEMP_TOTAL_LEN);

    /*处理流记录器*/
    this->SetStremIndexPlusOne(
            DissectResultLinkLayer::stream
            .Add(dissectResultBase,header->src,header->dst,LINKLAYER_FIELD_LENGTH_SRC_ADDR)
    );

    /*传递高层协议需要的数据,必须在AddNextLayer之前调用*/
    dissectResultBase->AddAdditional("linklayer_have_fcs",LINKLAYER_FIELD_LENGTH_TEMP_TOTAL_LEN,(void*)&this->haveFCS);

    if(this->header != nullptr)
        this->addNextLayer(dissectResultBase,(LINKLAYER_PROTOCOL_TYPE)*(quint16*)header->type,reserves);
}



//void* DissectResultLinkLayer::GetNextLayer(){
//    return this->protocol_family_network_layer;
//}

//DissectResultBase* DissectResultLinkLayer::GetDissectResultBase(){
//    return this->dissectResultBase;
//}

/*获取协议首部字段位置或值的方法*/
const quint8* DissectResultLinkLayer::GetSourceAddressPtr(){
    return this->header->src;
}

const quint8* DissectResultLinkLayer::GetDestinationAddressPtr(){
    return this->header->dst;
}

QString DissectResultLinkLayer::GetSourceAddressOriginalStr(){
    return this->getAddressOriginalStr(this->header->src);
}

QString DissectResultLinkLayer::GetDestinationAddressOriginalStr(){
    return this->getAddressOriginalStr(this->header->dst);
}

QString DissectResultLinkLayer::GetSourceAddressStr(){
    return this->getAddressStr(this->header->src);
}

QString DissectResultLinkLayer::GetDestinationAddressStr(){
    return this->getAddressStr(this->header->dst);
}

/*----处理LG/IG bit------*/
bool DissectResultLinkLayer::SourceAddressIsGroup(){
    return this->isGroupAddress(this->header->src);
}

bool DissectResultLinkLayer::DestinationAddressIsGroup(){
    return this->isGroupAddress(this->header->dst);
}

bool DissectResultLinkLayer::SourceAddressIsLocalAdministered(){
    return this->isLocalAdministeredAddress(this->header->src);
}

bool DissectResultLinkLayer::DestinationAddressIsLocalAdministered(){
    return this->isLocalAdministeredAddress(this->header->dst);
}

QString DissectResultLinkLayer::GetSourceAddressLGBitStr(){
    return this->getAddressLGBbitStr(this->header->src);
}

QString DissectResultLinkLayer::GetSourceAddressLGBit_meaning(){
    return this->getAddressLGbit_meaning(this->header->src);
}

QString DissectResultLinkLayer::GetSourceAddressLGBit_short_meaning(){
    return this->getAddressLGbit_short_meaning(this->header->src);
}

QString DissectResultLinkLayer::GetSourceAddressIGBitStr(){
    return this->getAddressIGBitStr(this->header->src);
}

QString DissectResultLinkLayer::GetSourceAddressIGBit_meaning(){
    return  this->getAddressIGbit_meaning(this->header->src);
}

QString DissectResultLinkLayer::GetSourceAddressIGBit_short_meaning(){
    return this->getAddressIGbit_short_meaning(this->header->src);
}

QString DissectResultLinkLayer::GetDestinationAddressLGBitStr(){
    return this->getAddressLGBbitStr(this->header->dst);
}

QString DissectResultLinkLayer::GetDestinationAddressLGBit_meaning(){
    return this->getAddressLGbit_meaning(this->header->dst);
}

QString DissectResultLinkLayer::GetDestinationAddressLGBit_short_meaning(){
    return this->getAddressLGbit_short_meaning(this->header->dst);
}

QString DissectResultLinkLayer::GetDestinationAddressIGBitStr(){
    return this->getAddressIGBitStr(this->header->dst);
}

QString DissectResultLinkLayer::GetDestinationAddressIGBit_meaning(){
    return this->getAddressIGbit_meaning(this->header->dst);
}

QString DissectResultLinkLayer::GetDestinationAddressIGBit_short_meaning(){
    return this->getAddressIGbit_short_meaning(this->header->dst);
}
/* --- 处理IG/LG bit 结束*/

const quint8* DissectResultLinkLayer::GetTypePtr(){
    return this->header->type;
}

QString DissectResultLinkLayer::GetTypeStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->header->type,LINKLAYER_FIELD_LENGTH_TYPE);
//    return QString("0x%1%2").arg(this->header->type[0],2,16,QLatin1Char('0')).arg(this->header->type[1],2,16,QLatin1Char('0'));
}

QString DissectResultLinkLayer::GetTypeName(){
    switch (*(quint16*)this->header->type) {
    case LINKLAYER_TYPE_ARP:
        return "arp";
    case LINKLAYER_TYPE_IPV4:
        return "ipv4";
    case LINKLAYER_TYPE_IPV6:
        return "ipv6";
    default:
        return "linklayer_nudeal_type";
    }
}

/*处理FCS*/
bool DissectResultLinkLayer::HaveFCS(){
    return this->haveFCS;
}

const quint8* DissectResultLinkLayer::GetFCSPtr(){
    return this->GetDissectResultBase()->GetData()
            + this->GetDissectResultBase()->GetPkthdr()->caplen - LINKLAYER_FIELD_LENGTH_FCS;
}

QString DissectResultLinkLayer::GetFCSStr(){
    return Converter::ConvertQuint8ArrayToHexStr(this->GetFCSPtr(),LINKLAYER_FIELD_LENGTH_FCS);
}

quint32 DissectResultLinkLayer::GetCalculatedFCS(){
    quint32 crc32_tab[] =
    {
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
    0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
    0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
    0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
    0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
    0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
    0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
    0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
    0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
    0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
    0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
    0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
    0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
    0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
    0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
    0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
    0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
    0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
    0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
    0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
    0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
    0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
    0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
    0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
    0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
    0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
    0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
    0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
    0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
    0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
    0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
    0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
    0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
    0x2d02ef8dL
    };
    quint32 crc32val = 0;
    crc32val ^= 0xFFFFFFFF;
    for (qint32 i = 0; i < (qint32)this->GetDissectResultBase()->GetPkthdr()->caplen - LINKLAYER_FIELD_LENGTH_FCS; i++)
        crc32val = crc32_tab[(crc32val ^ this->GetDissectResultBase()->GetData()[i]) & 0xFF] ^ ((crc32val >> 8) & 0x00FFFFFF);
    return ntohl(crc32val ^ 0xFFFFFFFF);
}

QString DissectResultLinkLayer::GetCalculatedFCSStr(){
    quint32 calculatedFCS = htonl(this->GetCalculatedFCS());
    return Converter::ConvertQuint8ArrayToHexStr((quint8*)&calculatedFCS,LINKLAYER_FIELD_LENGTH_FCS);
}


QString DissectResultLinkLayer::getAddressOriginalStr(quint8 *address){
    return Converter::ConvertQuint8ArrayToHexStr(address,LINKLAYER_FIELD_LENGTH_SRC_ADDR,":","");
}

QString DissectResultLinkLayer::getAddressStr(quint8 *address){
    /*
     * XX:XX:XX:XX:XX:XX
     * XX:XX:XX
     * XX:XX:XX:X
     * XX:XX:XX:XX:X
     */
    QString originalAddress = this->getAddressOriginalStr(address);
    if(ethernet_address_modify_hash.contains(originalAddress)){   //all
        return ethernet_address_modify_hash.value(originalAddress);
    }else if(ethernet_address_modify_hash.contains(originalAddress.left(13))){  //left 36 bit
        return originalAddress.replace(0,13,ethernet_address_modify_hash.value(originalAddress.left(13))+"_");
    }else if(ethernet_address_modify_hash.contains(originalAddress.left(10))){  //left 28 bit
        return originalAddress.replace(0,10,ethernet_address_modify_hash.value(originalAddress.left(10))+"_");
    }else if(ethernet_address_modify_hash.contains(originalAddress.left(8))){
        return originalAddress.replace(8,1,'_')
                .replace(0,8,ethernet_address_modify_hash.value(originalAddress.left(8)));
    }else{
        if(file_ethernet_address.exists() && file_ethernet_address.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&file_ethernet_address);
            stream.setAutoDetectUnicode(true);
            while(!stream.atEnd()){
                QStringList stringList = stream.readLine().split(' ');
                if(originalAddress == stringList.first().toLower() && stringList.length() > 1){
                    ethernet_address_modify_hash.insert(stringList.first().toLower(),stringList.at(1));
                    file_ethernet_address.close();
                    return stringList.at(1);
                }
            }
            file_ethernet_address.close();
        }

        if(file_ethernet_well_know_address.exists() && file_ethernet_well_know_address.open(QIODevice::ReadOnly | QIODevice::Text)){
            bool havefind = false;
            QTextStream stream(&file_ethernet_well_know_address);
            stream.setAutoDetectUnicode(true);
            while(!stream.atEnd()){
                QStringList stringList = stream.readLine().split(' ');
                if(stringList.length() > 1){
                    if(stringList.first().length() == 8){
                        if(stringList.first().toLower() == originalAddress.left(8)){
                            ethernet_address_modify_hash.insert(stringList.first().toLower(),stringList.at(1));
                            havefind = true;
                        }
                    }else if(stringList.first().length() == 20){
                        if(stringList.first().toLower() == originalAddress.left(13)){
                            ethernet_address_modify_hash.insert(stringList.first().toLower().left(13),stringList.at(1));
                            havefind = true;
                        }else if(stringList.first().toLower() == originalAddress.left(10)){
                            ethernet_address_modify_hash.insert(stringList.first().toLower().left(10),stringList.at(1));
                            havefind = true;
                        }else{
                            continue;
                        }
                    }else{
                        continue;
                    }
                }
            }
            if(!havefind)
                ethernet_address_modify_hash.insert(originalAddress,originalAddress);
            file_ethernet_well_know_address.close();
        }
        return this->getAddressStr(address);
    }
}

void DissectResultLinkLayer::addNextLayer(DissectResultBase *dissectResultBase, LINKLAYER_PROTOCOL_TYPE type,void *reserves){
    switch(type){
    case LINKLAYER_TYPE_IPV4:
        this->SetNextLayer((void*)(new DissectResultIpv4(dissectResultBase,reserves)));
        break;
    case LINKLAYER_TYPE_IPV6:
        this->SetNextLayer((void*)(new DissectResultIpv6(dissectResultBase,reserves)));
        break;
    case LINKLAYER_TYPE_ARP:
        this->SetNextLayer((void*)(new DissectResultArp(dissectResultBase,reserves)));
        break;
    default:
    {
        this->GetDissectResultBase()->SetSummery(QString("未处理链路层类型: %1")
                                            .arg(this->GetTypeStr())
                                            );
        this->SetNextLayer();
    }
    }
}

/*Private*/
bool DissectResultLinkLayer::isGroupAddress(const quint8 *address){
    if((address[0] & 0x01) == 0x01)
        return true;
    else
        return false;
}

bool DissectResultLinkLayer::isLocalAdministeredAddress(const quint8 *address){
    if((address[0] & 0x02) == 0x02)
        return true;
    else
        return false;
}

/*第一个字节的倒数第二位*/
QString DissectResultLinkLayer::getAddressLGBbitStr(const quint8 *address){
    return QString(".... ..%1. .... .... .... ....")
            .arg(this->isLocalAdministeredAddress(address)?1:0);
}

QString DissectResultLinkLayer::getAddressLGbit_short_meaning(const quint8 *address){
    if( this->isLocalAdministeredAddress(address) )
        return QString("Locally administered address");
    else
        return QString("Globally unique address");
}

QString DissectResultLinkLayer::getAddressLGbit_meaning(const quint8 *address){
    if( this->isLocalAdministeredAddress(address) )
        return QString("This is not the factory default");
    else
        return QString("Factory default");
}

/*第一个字节的最后一位*/
QString DissectResultLinkLayer::getAddressIGBitStr(const quint8 *address){
    return QString(".... ...%1 .... .... .... ....")
            .arg(this->isGroupAddress(address)?1:0);
}

QString DissectResultLinkLayer::getAddressIGbit_meaning(const quint8 *address){
    if( this->isGroupAddress(address) )
        return QString("Group address");
    else
        return QString("Individual address");
}

QString DissectResultLinkLayer::getAddressIGbit_short_meaning(const quint8 *address){
    if( this->isGroupAddress(address) )
        return QString("multicast/broadcast");
    else
        return QString("nuicast");
}
