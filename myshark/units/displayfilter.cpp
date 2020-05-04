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

bool DisplayFilter::Filte(DissectResultFrame *frame){
    this->mutex.tryLock();
    if( !this->filterStr.isEmpty() ){
        if( qAbs(frame->GetDissectResultBase()->GetAdditionalVal(TCP_STREAM)) - 1 == this->filterStr.toInt()){
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
