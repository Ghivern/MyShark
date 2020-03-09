#include "dissreseth.h"

DissResEth::DissResEth(qint64 no):DissRes(no)
{

}

void DissResEth::SetIpv6(){
    this->isIpv6 = true;
}

void DissResEth::SetMacSrc(uchar *macSrc){
    memcpy(this->macSrc,macSrc,EthfieldLen::MAC_ADDRESS);
}

void DissResEth::SetMacDst(uchar *macDst){
    memcpy(this->macDst,macDst,EthfieldLen::MAC_ADDRESS);
}

void DissResEth::SetIpSrc(uchar *ipSrc){
    memcpy(this->ipSrc,ipSrc,EthfieldLen::IP_ADDRESS);
}

void DissResEth::SetIpDst(uchar *ipDst){
    memcpy(this->ipDst,ipDst,EthfieldLen::IP_ADDRESS);
}

void DissResEth::SetIpv4Src(uchar *ipSrc){
    memcpy(this->ipSrc,ipSrc,EthfieldLen::IPV6_ADDRESS);
}

void DissResEth::SetIpv4Dst(uchar *ipDst){
    memcpy(this->ipDst,ipDst,EthfieldLen::IPV6_ADDRESS);
}

void DissResEth::SetSrcPort(ushort srcPort){
    this->srcPort = ntohs(srcPort);
}

void DissResEth::SetDstPort(ushort dstPort){
    this->dstPort = ntohs(dstPort);
}

QString DissResEth::GetStrMacSrc(){
    return QString::asprintf("%02x:%02x:%02x:%02x:%02x:%02x"
            ,this->macSrc[0]
            ,this->macSrc[1]
            ,this->macSrc[2]
            ,this->macSrc[3]
            ,this->macSrc[4]
            ,this->macSrc[5]
            );
}

QString DissResEth::GetStrMacDst(){
    return QString::asprintf("%02x:%02x:%02x:%02x:%02x:%02x"
            ,this->macDst[0]
            ,this->macDst[1]
            ,this->macDst[2]
            ,this->macDst[3]
            ,this->macDst[4]
            ,this->macDst[5]
            );
}

ushort DissResEth::GetSrcPort(){
    return this->srcPort;
}

ushort DissResEth::GetDstPort(){
    return this->dstPort;
}

QString DissResEth::GetStrIpSrc(){
    return QString::asprintf("%02x.%02x.%02x.%02x"
            ,this->ipSrc[0]
            ,this->ipSrc[1]
            ,this->ipSrc[2]
            ,this->ipSrc[3]
            );
}

QString DissResEth::GetStrIpDst(){
    return QString::asprintf("%02x.%02x.%02x.%02x"
            ,this->ipDst[0]
            ,this->ipDst[1]
            ,this->ipDst[2]
            ,this->ipDst[3]
            );
}
