#include "dissres.h"

bool DissRes::isFirstPacket = true;
timeval DissRes::firstTv;

DissRes::DissRes(qint64 no)
{
    this->no = no;
    this->headersLen = 0;
}

DissRes::~DissRes(){
    free(this->packet.data);
    free(this->packet.pkthdr);
}

void DissRes::SetPacket(const uchar *data,const pcap_pkthdr *pkthdr){
    this->packet.data = (uchar*)malloc(pkthdr->caplen);
    memcpy(this->packet.data,data,pkthdr->caplen);
    this->packet.pkthdr = (pcap_pkthdr*)malloc(sizeof(pcap_pkthdr));
    memcpy(this->packet.pkthdr,pkthdr,sizeof(pcap_pkthdr));
}

void DissRes::AddToProtocolStack(QString protocol){
    this->protocolStack.append(protocol);
}

void DissRes::AddToProtocolStackWithSE(QString protocol,qint32 len){
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

void DissRes::SetHeadersLen(qint32 headersLen){
    this->headersLen = headersLen;
}

void DissRes::AddHeadersLen(qint32 headerLen){
    this->headersLen += headerLen;
}

void DissRes::AddProPosition(QString proName, qint32 start, qint32 end){
    position_t p;
    p.start = start;
    p.end = end;
    this->positionStack.insert(proName,p);
}

const uchar* DissRes::GetData(){
    return this->packet.data;
}

const pcap_pkthdr* DissRes::GetPkthdr(){
    return this->packet.pkthdr;
}

void DissRes::SetMsg(QString msg){
    this->msg = msg;
}

qint64 DissRes::GetNo(){
    return this->no;
}

timeval DissRes::GetTimeval(){
    return this->packet.pkthdr->ts;
}

float DissRes::GetTimeSinceFirstPacket(){
    return (this->packet.pkthdr->ts.tv_sec - DissRes::firstTv.tv_sec)
            +
            (this->packet.pkthdr->ts.tv_usec - DissRes::firstTv.tv_usec)/1000000.0;
}

qint32 DissRes::GetCapLen(){
    return this->packet.pkthdr->caplen;
}

qint32 DissRes::GetCapLenBit(){
    return this->GetCapLen() * 8;
}

qint32 DissRes::GetLen(){
    return this->packet.pkthdr->len;
}

qint32 DissRes::GetLenBit(){
    return this->GetLen() * 8;
}

QString DissRes::GetTopProtocol(){
    if(protocolStack.isEmpty())
        return "";
    return this->protocolStack.last();
}

QString DissRes::GetProtocolByIndex(qint32 index){
    return this->protocolStack.at(index);
}

QList<QString>& DissRes::GetProtocolStack(){
    return this->protocolStack;
}

qint32 DissRes::GetHeadersLen(){
    return this->headersLen;
}

qint32 DissRes::GetProStart(QString proName){
    return this->positionStack.value(proName).start;
}

qint32 DissRes::GetProEnd(QString proName){
    if(this->positionStack.contains(proName))
        return this->positionStack.value(proName).end;
    else
        return 0;
}

qint32 DissRes::GetTopProEnd(){
    return this->GetProEnd(this->GetTopProtocol());
}

QString DissRes::GetMsg(){
    return this->msg;
}
