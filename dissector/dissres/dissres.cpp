#include "dissres.h"

bool DissRes::isFirstPacket = true;
timeval DissRes::firstTv;

DissRes::DissRes(qint64 no)
{
    this->no = no;
    this->headersLen = 0;
}

void DissRes::SetTv(timeval tv){
    if(DissRes::isFirstPacket){
        DissRes::firstTv.tv_sec = tv.tv_sec;
        DissRes::firstTv.tv_usec = tv.tv_usec;
        DissRes::isFirstPacket = false;
    }
    this->tv.tv_sec = tv.tv_sec;
    this->tv.tv_usec = tv.tv_usec;
}

void DissRes::SetCapLen(qint32 capLen){
    this->capLen = capLen;
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

void DissRes::SetMsg(QString msg){
    this->msg = msg;
}

qint64 DissRes::GetNo(){
    return this->no;
}

float DissRes::GetTimeSinceFirstPacket(){
    return (this->tv.tv_sec - DissRes::firstTv.tv_sec)
            +
            (this->tv.tv_usec - DissRes::firstTv.tv_usec)/1000000.0;
}

qint32 DissRes::GetCapLen(){
    return this->capLen;
}

QString DissRes::GetTopProtocol(){
    if(protocolStack.isEmpty())
        return "";
    return this->protocolStack.last();
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
