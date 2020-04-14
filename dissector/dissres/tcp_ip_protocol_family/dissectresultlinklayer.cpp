#include "dissectresultlinklayer.h"
#include <QDebug>

using namespace tcp_ip_protocol_family;

const QString DissectResultLinkLayer::relative_dir = "../others/";
const QString DissectResultLinkLayer::ethernet_address_file_path = relative_dir + "ethernet-addresses";
const QString DissectResultLinkLayer::ethernet_well_know_address_file_path = relative_dir + "ethernet-well-know-addresses";

QHash<QString,QString> DissectResultLinkLayer::ethernet_address_modify_hash;
QFile DissectResultLinkLayer::file_ethernet_address(ethernet_address_file_path);
QFile DissectResultLinkLayer::file_ethernet_well_know_address(ethernet_well_know_address_file_path);

DissectResultLinkLayer::DissectResultLinkLayer(DissectResultBase *dissectResultBase){
    this->protocol_family_network_layer = NULL;
    this->dissectResultBase = dissectResultBase;
    dissectResultBase->PushToProtocolList("ether",LINKLAYER_FIELD_LENGTH::LINKLAYER_FIELD_LENGTH_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ether");
    dissectResultBase->UpdateProtocolHeaderLengthCount(LINKLAYER_FIELD_LENGTH_TEMP_TOTAL_LEN);
    if(this->header != NULL)
        this->AddNextLayer(dissectResultBase,(LINKLAYER_PROTOCOL_TYPE)*(quint16*)header->type);
}

void DissectResultLinkLayer::AddNextLayer(DissectResultBase *dissectResultBase, LINKLAYER_PROTOCOL_TYPE type){
    switch(type){
    case LINKLAYER_TYPE_IPV4:
        this->protocol_family_network_layer = (void*)(new DissectResultIpv4(dissectResultBase));
        break;
    case LINKLAYER_TYPE_IPV6:
        this->protocol_family_network_layer = (void*)(new DissectResultIpv6(dissectResultBase));
        break;
    case LINKLAYER_TYPE_ARP:
        this->protocol_family_network_layer = (void*)(new DissectResultArp(dissectResultBase));
        break;
    default:
        this->protocol_family_network_layer = NULL;
    }
}

void* DissectResultLinkLayer::GetNextLayer(){
    return this->protocol_family_network_layer;
}

//DissectResultIpv4* DissectResultLinkLayer::GetNextLayerIpv4(){
//    return (DissectResultIpv4*)this->GetNextLayer();
//}

//DissectResultIpv6* DissectResultLinkLayer::GetNextLayerIpv6(){
//    return (DissectResultIpv6*)this->GetNextLayer();
//}

//DissectResultArp* DissectResultLinkLayer::GetNextLayerArp(){
//    return (DissectResultArp*)this->GetNextLayer();
//}



/*
 * 获取协议首部字段位置或值的方法
 */

const quint8* DissectResultLinkLayer::GetSourceAddressPtr(){
    return this->header->src;
}

const quint8* DissectResultLinkLayer::GetDestinationAddressPtr(){
    return this->header->dst;
}

QString DissectResultLinkLayer::GetSourceAddressOriginalStr(){
    return this->GetAddressOriginalStr(this->header->src);
}

QString DissectResultLinkLayer::GetDestinationAddressOriginalStr(){
    return this->GetAddressOriginalStr(this->header->dst);
}

QString DissectResultLinkLayer::GetSourceAddressStr(){
    return this->GetAddressStr(this->header->src);
}

QString DissectResultLinkLayer::GetDestinationAddressStr(){
    return this->GetAddressStr(this->header->dst);
}

bool DissectResultLinkLayer::SourceAddressIsGroup(){
    return this->IsGroupAddress(this->header->src);
}

bool DissectResultLinkLayer::DestinationAddressIsGroup(){
    return this->IsGroupAddress(this->header->dst);
}

bool DissectResultLinkLayer::SourceAddressIsLocalAdministered(){
    return this->IsLocalAdministeredAddress(this->header->src);
}

bool DissectResultLinkLayer::DestinationAddressIsLocalAdministered(){
    return this->IsLocalAdministeredAddress(this->header->dst);
}

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

//Private

QString DissectResultLinkLayer::GetAddressOriginalStr(quint8 *address){
    return Converter::ConvertQuint8ArrayToHexStr(address,LINKLAYER_FIELD_LENGTH_SRC_ADDR,":","");
}

QString DissectResultLinkLayer::GetAddressStr(quint8 *address){
    /*
     * XX:XX:XX:XX:XX:XX
     * XX:XX:XX
     * XX:XX:XX:X
     * XX:XX:XX:XX:X
     */
    QString originalAddress = this->GetAddressOriginalStr(address);
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
        return this->GetAddressStr(address);
    }
}

bool DissectResultLinkLayer::IsGroupAddress(const quint8 *address){
    if((address[0] & 0x01) == 0x01)
        return true;
    else
        return false;
}

bool DissectResultLinkLayer::IsLocalAdministeredAddress(const quint8 *address){
    if((address[0] & 0x02) == 0x02)
        return true;
    else
        return false;
}
