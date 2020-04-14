#include "dissectresultcommonstream.h"

DissectResultCommonStream::DissectResultCommonStream()
{
    this->streamIndexPlusOne = 0;
}

qint64 DissectResultCommonStream::GetOriginalStreamIndex(){
    return this->streamIndexPlusOne;
}

qint64 DissectResultCommonStream::GetStreamIndex(){
    return abs(this->streamIndexPlusOne) - 1;
}
