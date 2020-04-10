#include "streamrecorder.h"


StreamRecorder::StreamRecorder()
{

}

void StreamRecorder::Add(QString addressS,QString addressD,quint16 portS,quint16 portD,qint64 dissRes_index){
    qint32 streamItem_index = this->index.Add(addressS,addressD,portS,portD,this->streams.length());
    if(streamItem_index == this->streams.length()){
        this->streams.append(new StreamItem(addressS,addressD,portS,portD,dissRes_index,&this->streamIndex));
    }else{
        this->streams.at(streamItem_index)->Modify(dissRes_index,addressS,&this->streamIndex);
    }
}

void StreamRecorder::Add(QString addressS, QString addressD, qint64 dissRes_index){
    StreamRecorder::Add(addressS,addressD,0,0,dissRes_index);
}

qint64 StreamRecorder::GetStreamIndex(QString addressS, QString addressD, quint16 portS, quint16 portD){
    qint64 streamItem_index = this->index.GetStreamItemIndex(addressS,addressD,portS,portD);
    if(streamItem_index != -1)
        return this->streams.at(streamItem_index)->GetStreamIndex(addressS);
    else
        return -1;
}

qint64 StreamRecorder::GetStreamItemCount(){
    return this->streams.length();
}

StreamItem* StreamRecorder::GetStreamItemByIndex(qint64 index){
    return this->streams.at(index);
}
