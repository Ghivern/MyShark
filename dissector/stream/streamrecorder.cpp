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
