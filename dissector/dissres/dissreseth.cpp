#include "dissreseth.h"

bool DissResEth::isFirstPacket = true;
timeval DissResEth::firstTv;


DissResEth::DissResEth(qint64 no)
{
    this->no = no;
    this->headersLen = 0;
}

void DissResEth::SetIpv6(){
    this->isIpv6 = true;
}

void DissResEth::SetMacSrc(uchar *macSrc){
    memcpy(this->macSrc,macSrc,fieldLen::MAC_ADDRESS);
}

void DissResEth::SetMacDst(uchar *macDst){
    memcpy(this->macDst,macDst,fieldLen::MAC_ADDRESS);
}

void DissResEth::SetTv(timeval tv){
    if(DissResEth::isFirstPacket){
        DissResEth::firstTv.tv_sec = tv.tv_sec;
        DissResEth::firstTv.tv_usec = tv.tv_usec;
        DissResEth::isFirstPacket = false;
    }
    this->tv.tv_sec = tv.tv_sec;
    this->tv.tv_usec = tv.tv_usec;
}

void DissResEth::SetDisplayTv(timeval tv){
    this->displayTv.tv_sec = tv.tv_sec;
    this->displayTv.tv_usec = tv.tv_usec;
}

void DissResEth::SetIpSrc(uchar *ipSrc){
    memcpy(this->ipSrc,ipSrc,fieldLen::IP_ADDRESS);
}

void DissResEth::SetIpDst(uchar *ipDst){
    memcpy(this->ipDst,ipDst,fieldLen::IP_ADDRESS);
}

void DissResEth::SetIpv4Src(uchar *ipSrc){
    memcpy(this->ipSrc,ipSrc,fieldLen::IPV6_ADDRESS);
}

void DissResEth::SetIpv4Dst(uchar *ipDst){
    memcpy(this->ipDst,ipDst,fieldLen::IPV6_ADDRESS);
}

void DissResEth::SetCapLen(qint32 capLen){
    this->capLen = capLen;
}

void DissResEth::AddToProtocolStack(QString protocol){
    this->protocolStack.append(protocol);
}

void DissResEth::AddToProtocolStackWithSE(QString protocol,qint32 len){
    qint32 preEnd = this->GetTopProEnd();
    qint32 start,end;
    if(preEnd == 0)
        start = preEnd;
    else
        start = preEnd + 1;
    if(len == 0)
        end = start + len;
    else
        end = start +len -1;
    this->AddProPosition(protocol,start,end);
    this->AddToProtocolStack(protocol);
}

void DissResEth::SetHeadersLen(qint32 headersLen){
    this->headersLen = headersLen;
}

void DissResEth::AddHeadersLen(qint32 headerLen){
    this->headersLen += headerLen;
}

void DissResEth::AddProPosition(QString proName, qint32 start, qint32 end){
    position_t p;
    p.start = start;
    p.end = end;
    this->positionStack.insert(proName,p);
}

void DissResEth::SetSrcPort(ushort srcPort){
    this->srcPort = ntohs(srcPort);
}

void DissResEth::SetDstPort(ushort dstPort){
    this->dstPort = ntohs(dstPort);
}

void DissResEth::SetMsg(QString msg){
    this->msg = msg;
}


qint64 DissResEth::GetNo(){
    return this->no;
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

float DissResEth::GetTimeSinceFirstPacket(){
    return (this->tv.tv_sec - DissResEth::firstTv.tv_sec)
            +
            (this->tv.tv_usec - DissResEth::firstTv.tv_usec)/1000000.0;
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

qint32 DissResEth::GetCapLen(){
    return this->capLen;
}

QString DissResEth::GetTopProtocol(){
    if(protocolStack.isEmpty())
        return "";
    return this->protocolStack.last();
}

QList<QString>& DissResEth::GetProtocolStack(){
    return this->protocolStack;
}

qint32 DissResEth::GetHeadersLen(){
    return this->headersLen;
}

qint32 DissResEth::GetProStart(QString proName){
    return this->positionStack.value(proName).start;
}

qint32 DissResEth::GetProEnd(QString proName){
    if(this->positionStack.contains(proName))
        return this->positionStack.value(proName).end;
    else
        return 0;
}

qint32 DissResEth::GetTopProEnd(){
    return this->GetProEnd(this->GetTopProtocol());
}

ushort DissResEth::GetSrcPort(){
    return this->srcPort;
}

ushort DissResEth::GetDstPort(){
    return this->dstPort;
}

QString DissResEth::GetMsg(){
    return this->msg;
}
