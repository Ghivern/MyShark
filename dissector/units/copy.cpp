#include "copy.h"

Copy::Copy(quint32 dataLen)
{
    this->startPosition = 0;
    this->dataLen = dataLen;
    this->data = new quint8[dataLen];
}

Copy& Copy::Append(const quint8 *src, quint32 len){
    memcpy(this->data + this->startPosition,src,len);
    this->startPosition += len;
    return *this;
}

quint8* Copy::GetData(){
    return this->data;
}
