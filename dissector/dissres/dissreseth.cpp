#include "dissreseth.h"

DissResEth::DissResEth(qint64 no):DissRes(no){}

void DissResEth::SetIpVersion(qint32 version){
    this->version = version;
}

void DissResEth::SetMacSrc(uchar *macSrc){
    memcpy(this->macSrc,macSrc,6);
}

void DissResEth::SetMacDst(uchar *macDst){
    memcpy(this->macDst,macDst,6);
}

void DissResEth::SetEthCRCRes(bool res){
    this->ethCRCRes = res;
}

void DissResEth::SetIpSrc(uchar *ipSrc){
    memcpy(this->ipSrc,ipSrc,IP_LENS::SOURCEIP);
}

void DissResEth::SetIpDst(uchar *ipDst){
    memcpy(this->ipDst,ipDst,IP_LENS::DESTIP);
}

void DissResEth::SetIpv6Src(uchar *ipSrc){
    memcpy(this->ipSrc,ipSrc,IPV6_LEN::SRC_ADDRESS);
}

void DissResEth::SetIpv6Dst(uchar *ipDst){
    memcpy(this->ipDst,ipDst,IPV6_LEN::DST_ADDRESS);
}

void DissResEth::SetSrcPort(ushort srcPort){
    this->srcPort = ntohs(srcPort);
}

void DissResEth::SetDstPort(ushort dstPort){
    this->dstPort = ntohs(dstPort);
}

void DissResEth::SetIpTotalLen(qint32 ipTotalLen){
    this->ipTotalLen = ipTotalLen;
}

void DissResEth::SetIpHeaderLen(quint8 len){
    this->ipHeaderLen = len;
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

bool DissResEth::GetEthCRCRes(){
    return this->ethCRCRes;
}

ushort DissResEth::GetSrcPort(){
    return this->srcPort;
}

ushort DissResEth::GetDstPort(){
    return this->dstPort;
}

qint32 DissResEth::GetIpTotalLen(){
    return this->ipTotalLen;
}

quint8 DissResEth::GetIpHeaderLen(){
    return this->ipHeaderLen;
}

quint32 DissResEth::GetIpSrcAddress(){
    quint32 address;
    memcpy(&address,this->ipSrc,IP_LENS::SOURCEIP);
    return ntohl(address);
}

quint32 DissResEth::GetIpDstAddress(){
    quint32 address;
    memcpy(&address,this->ipDst,IP_LENS::DESTIP);
    return ntohl(address);
}

quint32 DissResEth::GetIpSrcAddressNetworkOrder(){
    return GetIpAddressNetworkOrder(this->ipSrc);
}

quint32 DissResEth::GetIpDstAddressNetworkOrder(){
    return GetIpAddressNetworkOrder(this->ipDst);
}

QString DissResEth::GetStrIpSrc(){
    return QString::asprintf("%d.%d.%d.%d"
            ,this->ipSrc[0]
            ,this->ipSrc[1]
            ,this->ipSrc[2]
            ,this->ipSrc[3]
            );
}

QString DissResEth::GetStrIpDst(){
    return QString::asprintf("%d.%d.%d.%d"
            ,this->ipDst[0]
            ,this->ipDst[1]
            ,this->ipDst[2]
            ,this->ipDst[3]
            );
}

/*遇到非0
 *     (1)zeroCount <= 0,无特殊处理
 *     (2)zeroCount == 1,zeroCount清零,并在添加非0数据前添加0
 *     (3)zeroCount > 1,使用::代替连续0,zeroCount置-1,zeroEndPosition设为index
 * 遇到0
 *      (1)zeroCount < 0,无特殊处理
 *      (2)zeroCount >= 0 ,zeroCount++;
*/
QString DissResEth::GetStrIpv6Address(quint8 *address){
    quint16 *start = (quint16*)address;
    quint8 *temp;
    qint32 zeroCount = 0;
    qint32 zeroEndPosition = 0;
    bool firstIsZero = false;
    QString strAddress;
    for(qint32 index = 0; index < IPV6_LEN::ADDRESS/2; index++){
        temp = (quint8*)(start+index);
        if(start[index] == 0){
            if(index == 0)
                firstIsZero = true;
            if(zeroCount < 0){
                strAddress.append(QString::asprintf("%02x%02x:",temp[0],temp[1]));
            }else{
                zeroCount++;
            }
        }else{
            if(zeroCount == 0 || zeroCount == -1){
               strAddress.append(QString::asprintf("%02x%02x:",temp[0],temp[1]));
            }else if(zeroCount == 1){
                zeroCount = 0;
                strAddress.append(QString::asprintf("%d:",0));
                strAddress.append(QString::asprintf("%02x%02x:",temp[0],temp[1]));
            }else{
                if(firstIsZero)
                    strAddress.append("::").append(QString::asprintf("%02x%02x:",temp[0],temp[1]));
                else
                    strAddress.append(":").append(QString::asprintf("%02x%02x:",temp[0],temp[1]));
                zeroCount = -1;
                zeroEndPosition = index-1;
            }
        }
    }
    if(zeroEndPosition != IPV6_LEN::ADDRESS/2 - 1)
        strAddress.remove(strAddress.length()-1,1);
    if(zeroCount == IPV6_LEN::ADDRESS/2)
        strAddress.append("::");
    return strAddress;
}

QString DissResEth::GetStrIpv6Src(){
    return GetStrIpv6Address(this->ipSrc);
}

QString DissResEth::GetStrIpv6Dst(){
    return GetStrIpv6Address(this->ipDst);
}


void DissResEth::GetIpv6SrcAddress(quint64 *high,quint64 *low){
    GetIpv6Address(this->ipSrc,high,low);
}

void DissResEth::GetIpv6DstAddress(quint64 *high,quint64 *low){
    GetIpv6Address(this->ipDst,high,low);
}

void DissResEth::GetIpv6SrcAddressNetworkOrder(quint64 *high,quint64 *low){
    GetIpv6AddressNetworkOrder(this->ipSrc,high,low);
}

void DissResEth::GetIpv6DstAddressNetworkOrder(quint64 *high,quint64 *low){
    GetIpv6AddressNetworkOrder(this->ipDst,high,low);
}

qint32 DissResEth::ipVersion(){        
    return this->version;
}

QString DissResEth::GetStrSrc(){
    qint32 v = this->ipVersion();
    switch (v) {
    case 0:
        return this->GetStrMacSrc();
    case 4:
        return this->GetStrIpSrc();
    case 6:
        return this->GetStrIpv6Src();
    default:
        return "Nuknow";
    }
}

QString DissResEth::GetStrDst(){
    qint32 v = this->ipVersion();
    switch (v) {
    case 0:
        return this->GetStrMacDst();
    case 4:
        return this->GetStrIpDst();
    case 6:
        return this->GetStrIpv6Dst();
    default:
        return "Nuknow";
    }
}


//protected 方法
quint32 DissResEth::GetIpAddressNetworkOrder(quint8 *address){
    return *((quint32*)address);
}

void DissResEth::GetIpv6Address(quint8 *address, quint64 *high, quint64 *low){
    quint32 *tempPtr = (quint32*)address;
    quint32 temp;

    temp = ntohl(tempPtr[3]);
    memcpy(low,&temp,sizeof(quint64)/2);

    temp = ntohl(tempPtr[2]);
    memcpy(low + sizeof(quint64)/2,&temp ,sizeof(quint64)/2);

    temp = ntohl(tempPtr[1]);
    memcpy(high,&temp,sizeof(quint64)/2);

    temp = ntohl(tempPtr[0]);
    memcpy(high + sizeof(quint64)/2,&temp ,sizeof(quint64)/2);
}

void DissResEth::GetIpv6AddressNetworkOrder(quint8 *address, quint64 *high, quint64 *low){
    memcpy(low,address,IPV6_LEN::SRC_ADDRESS/2);
    memcpy(high,address+IPV6_LEN::SRC_ADDRESS/2,IPV6_LEN::SRC_ADDRESS/2);
}
