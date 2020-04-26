#include "streamtcp2.h"
#include <QtDebug>

StreamTcp2::StreamTcp2()
{

}

void StreamTcp2::ClearWindow(qint64 streamIndexPlusOne){
    (*this->allWindow.find(streamIndexPlusOne)).windowVal = 0;
    (*this->allWindow.find(streamIndexPlusOne)).windowScale = 0;

    (*this->allWindow.find(streamIndexPlusOne)).flags = 0;

    (*this->allWindow.find(streamIndexPlusOne)).baseSeq = 0;
    (*this->allWindow.find(streamIndexPlusOne)).nextSeq = 0;
    (*this->allWindow.find(streamIndexPlusOne)).maxSeqToBeAcked = 0;

    (*this->allWindow.find(streamIndexPlusOne)).lastAck = 0;

    (*this->allWindow.find(streamIndexPlusOne)).lastSegmentFlags = 0;
    (*this->allWindow.find(streamIndexPlusOne)).dupAckNum = 0;
}

qint64 StreamTcp2::AddWithWindow(DissectResultBase *dissectResultBase,quint8 *srcAddr,quint8 *dstAddr,qint32 addr_size,quint8 *srcPort,quint8 *dstPort,qint32 port_size){
    qint64 streamIndexPlusOne =  this->Add(dissectResultBase,srcAddr,dstAddr,addr_size,srcPort,dstPort,port_size);

    bool SYN = dissectResultBase->GetAdditionalVal(TCP_ISSYN) == 1 ? true:false;
    bool FIN = dissectResultBase->GetAdditionalVal(TCP_ISFIN) == 1 ? true:false;
    bool RST = dissectResultBase->GetAdditionalVal(TCP_ISRST) == 1 ? true:false;
    bool NOT_ACK_OR_FIN_OR_RST = !(SYN || FIN || RST);
    bool ACK = dissectResultBase->GetAdditionalVal(TCP_ISACK) == 1 ? true:false;

    quint32 seq = dissectResultBase->GetAdditionalVal(TCP_SEQ_VAL);
    quint32 ack = dissectResultBase->GetAdditionalVal(TCP_ACK_VAL);
    quint32 segLen = dissectResultBase->GetAdditionalVal(TCP_PAYLOAD_LEN);
    qint32 windowVal = dissectResultBase->GetAdditionalVal(TCP_WINDOW);


    if( !allWindow.contains(streamIndexPlusOne) ){
        Window aWindow;
        Window aContraryWindow;
        this->allWindow.insert(streamIndexPlusOne,aWindow);
        this->allWindow.insert(-streamIndexPlusOne,aContraryWindow);
    }

    /*处理baseSeq*/
    if( ( this->allWindow.value(streamIndexPlusOne).flags & TCP_S_BASE_SEQ_SET ) && SYN && !ACK ){
        this->ClearWindow(streamIndexPlusOne);
        this->ClearWindow(-streamIndexPlusOne);
    }

    if( !( this->allWindow.value(streamIndexPlusOne).flags & TCP_S_BASE_SEQ_SET ) ){
        if( SYN ){
            (*this->allWindow.find(streamIndexPlusOne)).baseSeq = seq;
            (*this->allWindow.find(streamIndexPlusOne)).flags |= ACK ? TCP_S_SAW_SYNACK : TCP_S_SAW_SYN;
        }else{
            (*this->allWindow.find(streamIndexPlusOne)).baseSeq = seq - 1;
        }
        (*this->allWindow.find(streamIndexPlusOne)).flags |= TCP_S_BASE_SEQ_SET;
    }

    if( !( this->allWindow.value(-streamIndexPlusOne).flags & TCP_S_BASE_SEQ_SET) && ACK ){
        (*this->allWindow.find(-streamIndexPlusOne)).baseSeq = ack - 1;
        (*this->allWindow.find(-streamIndexPlusOne)).flags |= TCP_S_BASE_SEQ_SET;
    }

    /*TCP_A_ZERO_WINDOW_PROBE*/
    if( segLen == 1 && seq == this->allWindow.value(streamIndexPlusOne).nextSeq
            && this->allWindow.value(-streamIndexPlusOne).windowVal == 0){
        dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_ZERO_WINDOW_PROBE);
        goto finished;
    }

    if( windowVal == 0 && NOT_ACK_OR_FIN_OR_RST ){
        dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_ZERO_WINDOW);
    }

    if( this->allWindow.value(streamIndexPlusOne).nextSeq
            && seq > this->allWindow.value(streamIndexPlusOne).nextSeq
            && !RST ){
        dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_LOST_PACKET);
    }

    if( ( segLen == 0 || segLen == 1)
            && seq == this->allWindow.value(streamIndexPlusOne).nextSeq - 1
            && NOT_ACK_OR_FIN_OR_RST){
        dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_KEEP_ALIVE);
    }

    /*TCP_A_KEEP_ALIVE_ACK*/
    if( segLen == 0
            && this->allWindow.value(streamIndexPlusOne).windowVal
            && windowVal == this->allWindow.value(streamIndexPlusOne).windowVal
            && (this->allWindow.value(-streamIndexPlusOne).lastSegmentFlags & TCP_A_KEEP_ALIVE)){
        dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_KEEP_ALIVE_ACK);
    }
    finished:

    /*更新数据*/
    quint32 nextseq = seq + segLen;
    if( SYN || FIN ){
        nextseq += 1;
    }
    quint32 segmentFlag = dissectResultBase->GetAdditionalVal(TCP_STATUS2);
    if( nextseq > this->allWindow.value(streamIndexPlusOne).nextSeq
            || !this->allWindow.value(streamIndexPlusOne).nextSeq ){
        if( !( segmentFlag & TCP_A_ZERO_WINDOW_PROBE) ){
            (*this->allWindow.find(streamIndexPlusOne)).nextSeq = nextseq;
        }
    }

    if( seq == this->allWindow.value(streamIndexPlusOne).maxSeqToBeAcked
            || !this->allWindow.value(streamIndexPlusOne).maxSeqToBeAcked){
        if( !( segmentFlag & TCP_A_ZERO_WINDOW_PROBE) ){
            (*this->allWindow.find(streamIndexPlusOne)).maxSeqToBeAcked = this->allWindow.value(streamIndexPlusOne).nextSeq;
        }
    }

    (*this->allWindow.find(streamIndexPlusOne)).windowVal = windowVal;
    (*this->allWindow.find(streamIndexPlusOne)).lastAck = ack;
    (*this->allWindow.find(streamIndexPlusOne)).lastSegmentFlags = segmentFlag;

    return streamIndexPlusOne;
}

quint32 StreamTcp2::GetBaseSeq(qint64 streamIndexPlusOne){
    if( !this->allWindow.contains(streamIndexPlusOne) )
        qDebug() << "哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈哈和哈哈哈哈哈哈哈哈";
    return this->allWindow.value(streamIndexPlusOne).baseSeq;
}

quint32 StreamTcp2::GetWindowMultiplier(qint64 streamIndexPlusOne){
    return qPow(2,this->allWindow.value(streamIndexPlusOne).windowScale);
}
