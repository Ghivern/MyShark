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

//Set Method
void DissRes::SetPacket(const uchar *data,const pcap_pkthdr *pkthdr){
    this->packet.data = (uchar*)malloc(pkthdr->caplen);
    memcpy(this->packet.data,data,pkthdr->caplen);
    this->packet.pkthdr = (pcap_pkthdr*)malloc(sizeof(pcap_pkthdr));
    memcpy(this->packet.pkthdr,pkthdr,sizeof(pcap_pkthdr));
    if(isFirstPacket){
        firstTv.tv_sec = pkthdr->ts.tv_sec;
        firstTv.tv_usec = pkthdr->ts.tv_usec;
        isFirstPacket = false;
    }
}

void DissRes::SetMsg(QString msg){
    this->msg = msg;
}

void DissRes::SetHeadersLen(qint32 headersLen){
    this->headersLen = headersLen;
}

//Add Methods
void DissRes::AddToProtocolStackWithSE(QString protocol,qint32 len,bool hide){
    if(!this->proExist(protocol)){
        qint32 preProEnd = this->getTopProEnd();
        qint32 start,end;
        if(preProEnd == 0)
            start = preProEnd;
        else
            start = preProEnd + 1;
        if(len == 0)
            end = start;
        else
            end = start +len -1;
        this->addProPosition(protocol,start,end);
        if(!hide)
            this->addToProtocolStack(protocol);
    }
}

void DissRes::ResetProtocolStackAndPosition(QString protocol, qint32 newLen){
    if(this->proExist(protocol)){
        qint32 proStart = this->getTopProStart();
        qint32 proEnd;
        if(newLen == 0)
            proEnd = proStart;
        else
            proEnd = proStart + newLen -1;
        this->positionStack.remove(protocol);
        this->addProPosition(protocol,proStart,proEnd);
    }
}

void DissRes::AddHeadersLen(qint32 headerLen){
    this->headersLen += headerLen;
}


//GetMethods
const uchar* DissRes::GetData(){
    return this->packet.data;
}

const pcap_pkthdr* DissRes::GetPkthdr(){
    return this->packet.pkthdr;
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

qint32 DissRes::GetHeadersLen(){
    return this->headersLen;
}

QString DissRes::GetMsg(){
    return this->msg;
}

QString DissRes::GetTopProtocol(){
    if(protocolStack.isEmpty())
        return "";
    return this->protocolStack.last();
}

qint32 DissRes::GetProtocolStackLen(){
    return this->protocolStack.length();
}

QString DissRes::GetProtocolByIndex(qint32 index){
    return this->protocolStack.at(index);
}

QList<QString>& DissRes::GetProtocolStack(){
    return this->protocolStack;
}

qint32 DissRes::GetProStart(QString proName){
    if(this->positionStack.contains(proName))
        return this->positionStack.value(proName).start;
    else
        return 0;
}

const quint8* DissRes::GetProStartPtr(QString proName){
    return this->GetData() + this->GetProStart(proName);
}

qint32 DissRes::GetProEnd(QString proName){
    if(this->positionStack.contains(proName))
        return this->positionStack.value(proName).end;
    else
        return 0;
}

const quint8* DissRes::GetProEndPtr(QString proName){
    return this->GetData() + this->GetProEnd(proName);
}


//protected Methods
void DissRes::addToProtocolStack(QString protocol){
    this->protocolStack.append(protocol);
}

void DissRes::addProPosition(QString proName, qint32 start, qint32 end){
    position_t p;
    p.start = start;
    p.end = end;
    this->positionStack.insert(proName,p);
}

qint32 DissRes::getTopProEnd(){
    return this->GetProEnd(this->GetTopProtocol());
}

qint32 DissRes::getTopProStart(){
    return this->GetProStart(this->GetTopProtocol());
}

bool DissRes::proExist(QString proName){
    if(this->positionStack.contains(proName))
        return true;
    else
        return false;
}
