#include "displayfilter.h"

DisplayFilter::DisplayFilter()
{
    this->filterStr = "";
    this->displayedCount = 0;
}

bool DisplayFilter::SetFilter(QString filterStr){
    this->mutex.tryLock();
    this->filterStr = filterStr;
    this->displayedCount = 0;
    this->mutex.unlock();
    return true;
}

bool DisplayFilter::FilterIsValied(QString filterStr){
    bool allNumber = true;
    if( filterStr.isEmpty() )
        return true;
    for( qint32 index = 0; index < filterStr.length(); index++){
        if( filterStr.at(index).isNumber() ){
            continue;
        }else{
            allNumber = false;
            break;
        }
    }
    if( allNumber ){
        this->mutex.tryLock();
        this->filterStr = filterStr;
        this->displayedCount = 0;
        this->mutex.unlock();
        return true;
    }else{
        return false;
    }
}

bool DisplayFilter::Filte(DissectResultFrame *frame){
    TcpInfo &tcpInfo = *(TcpInfo*)frame->GetDissectResultBase()->GetAdditionalVal(TCP_INFO_PTR);
    this->mutex.tryLock();
    if( !this->filterStr.isEmpty() ){
        if( frame->GetDissectResultBase()->ContainProtocol("tcp")
                && qAbs(tcpInfo.streamPlusOne) - 1 == this->filterStr.toInt()){
            this->displayedCount++;
            this->mutex.unlock();
            return true;
        }else{
            this->mutex.unlock();
            return false;
        }
    }else{
        this->displayedCount++;
        this->mutex.unlock();
        return true;
    }
}

qint64 DisplayFilter::GetDisplayedCount(){
    return this->displayedCount;
}
