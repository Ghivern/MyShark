#include "dissectresultlinklayer.h"

using namespace tcp_ip_protocol_family;

const QString DissectResultLinkLayer::relatice_dir = "../../others/";
const QString DissectResultLinkLayer::ethernet_address_file_path = relatice_dir + "ethernet_address_file";
const QString DissectResultLinkLayer::well_know_ethernet_address_file_path = relatice_dir + "well_know_ethernet_address_file";
const QString DissectResultLinkLayer::ethernet_manufacturers_file_path = relatice_dir + "ethernet_manufacturers_file";

QHash<QString,QString> DissectResultLinkLayer::ethernet_address_modify_hash;
QFile DissectResultLinkLayer::file_ethernet_address(ethernet_address_file_path);
QFile DissectResultLinkLayer::file_well_know_ethernet_address(well_know_ethernet_address_file_path);
QFile DissectResultLinkLayer::file_ethernet_manufacturers(ethernet_manufacturers_file_path);

DissectResultLinkLayer::DissectResultLinkLayer(DissectResultBase *dissectResultBase){
    this->protocol_family_network_layer = NULL;
    this->dissectResultBase = dissectResultBase;
    dissectResultBase->PushToProtocolList("ether",LINKLAYER_FIELD_LENGTH::LINKLAYER_TEMP_TOTAL_LEN);
    this->header = (struct header_t*)dissectResultBase->GetProtocolHeaderStartPtrByName("ether");
    dissectResultBase->UpdateProtocolHeaderLengthCount(LINKLAYER_TEMP_TOTAL_LEN);
    if(this->header != NULL)
        this->AddNextLayer(dissectResultBase,(LINKLAYER_PROTOCOL_TYPE)*(quint16*)header->type);
}

void DissectResultLinkLayer::AddNextLayer(DissectResultBase *dissectResultBase, LINKLAYER_PROTOCOL_TYPE type){
    switch(type){
    case LINKLAYER_IPV4:
        this->protocol_family_network_layer = (void*)(new DissectResultIpv4(dissectResultBase));
        break;
    case LINKLAYER_IPV6:
        this->protocol_family_network_layer = (void*)(new DissectResultIpv6(dissectResultBase));
        break;
    case LINKLAYER_ARP:
        this->protocol_family_network_layer = (void*)(new DissectResultArp(dissectResultBase));
        break;
    }
}

void* DissectResultLinkLayer::GetNextLayer(){
    return this->protocol_family_network_layer;
}

DissectResultIpv4* DissectResultLinkLayer::GetNextLayerIpv4(){
    return (DissectResultIpv4*)this->GetNextLayer();
}

DissectResultIpv6* DissectResultLinkLayer::GetNextLayerIpv6(){
    return (DissectResultIpv6*)this->GetNextLayer();
}

DissectResultArp* DissectResultLinkLayer::GetNextLayerArp(){
    return (DissectResultArp*)this->GetNextLayer();
}

quint8* DissectResultLinkLayer::GetSourceAddressPtr(){
    return this->header->src;
}

QString DissectResultLinkLayer::GetSourceAddressOriginalStr(){
    QString originalAddress = "";
    for(qint32 index = 0; index < LINKLAYER_SRC_ADDR; index++){
        originalAddress =  originalAddress.append("%1").arg(this->header->src[index],2,16,QLatin1Char('0'));
        if(index != LINKLAYER_SRC_ADDR - 1)
            originalAddress.append(":");
    }
    return originalAddress;
}

QString DissectResultLinkLayer::GetSourceAddressStr(){
    QString originalAddress = this->GetSourceAddressOriginalStr();
    if(ethernet_address_modify_hash.contains(originalAddress)){
        return ethernet_address_modify_hash.value(originalAddress);
    }else if(ethernet_address_modify_hash.contains(originalAddress.left(8))){
        return originalAddress.replace(0,8,ethernet_address_modify_hash.value(originalAddress.left(8)));
    }else{
        if(file_ethernet_address.exists() && file_ethernet_address.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream stream(&file_ethernet_address);
            stream.setAutoDetectUnicode(true);
            while(!stream.atEnd()){
                QString aline = stream.readLine();
                QStringList stringList = aline.split(' ',QString::SkipEmptyParts);
                if(originalAddress == stringList.first()){
                    ethernet_address_modify_hash.insert(stringList.first(),stringList.last());
                    goto recursion;
                }
            }
            file_ethernet_address.close();
        }

        if(file_well_know_ethernet_address.exists() && file_well_know_ethernet_address.open(QIODevice::ReadOnly)){

        }

        if(file_ethernet_manufacturers.exists() && file_ethernet_manufacturers.open(QIODevice::ReadOnly)){

        }

        return originalAddress;

        recursion:
        return this->GetSourceAddressStr();
    }
}
