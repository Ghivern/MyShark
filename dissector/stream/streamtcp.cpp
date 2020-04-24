#include "streamtcp.h"
#include <QDebug>

StreamTcp::StreamTcp() {}

/*
 * 应该解析出来的结果
 * 1.有效数据开始的位置(ptr)
 * 2.前一个报文的索引
 * 3.Tcp报文的状态
 */
qint64 StreamTcp::AddWithWindow(DissectResultBase *dissectResultBase, quint8 *srcAddr, quint8 *dstAddr, qint32 addr_size, quint8 *srcPort, quint8 *dstPort, qint32 port_size){

    quint32 len = dissectResultBase->GetAdditionalVal(TCP_PAYLOAD_LEN);
    quint32 ack = dissectResultBase->GetAdditionalVal(TCP_ACK_VAL);

    quint64 index = dissectResultBase->GetIndex();
    qint64 streamIndexPlusOne =  this->Add(dissectResultBase,srcAddr,dstAddr,addr_size,srcPort,dstPort,port_size);


    QStack<qint32> waitforRemove; /*已经重组的乱序的包*/

    this->dealBaseSeq(dissectResultBase,streamIndexPlusOne);


    /*处理Ack*/
    if(this->acks.contains(ack)){
        (*this->acks.find(ack)).append(index);
    }else{
        QList<quint64> packetList;
        packetList.append(index);
        this->acks.insert(ack,packetList);
    }

    if(len == 0)
        return streamIndexPlusOne;
    else
        this->addSegmentToWindow(dissectResultBase,streamIndexPlusOne);


    return streamIndexPlusOne;
}

quint32 StreamTcp::GetBaseSeq(qint64 streamIndexPlusOne){
    return (*this->windows.find(streamIndexPlusOne)).baseSeq;
}

quint16 StreamTcp::GetWindowMultiplier(qint64 streamIndexPlusOne){
    return this->windows.value(streamIndexPlusOne).windowMultiplier;
}

/*若重新连接则返回true*/
bool StreamTcp::dealBaseSeq(DissectResultBase *dissectResultBase,qint64 streamIndexPlusOne){
    quint32 seq = dissectResultBase->GetAdditionalVal(TCP_SEQ_VAL);
    quint32 ack = dissectResultBase->GetAdditionalVal(TCP_ACK_VAL);
    bool SYN = dissectResultBase->GetAdditionalVal(TCP_ISSYN) == 1 ? true : false;
    bool ACK = dissectResultBase->GetAdditionalVal(TCP_ISACK) == 1 ? true : false;
    qint32 windowMultiplier = dissectResultBase->GetAdditionalVal(TCP_WINDOW_MULTIPLIER);
    bool contain_window = this->windows.contains(streamIndexPlusOne);
    struct window_t window;
    struct window_t contrary_window;
    if( !contain_window ){
        if(SYN && !ACK){                         /*SYN SYN ACK*/
            window.baseSeq = seq;
            window.maxSeq = seq + 1;
            window.windowMultiplier = qAbs(windowMultiplier);
        }else if(SYN && ACK){                    /*SYN ACK ACK*/
            window.baseSeq = seq;
            window.maxSeq = seq + 1;
            window.windowMultiplier = qAbs(windowMultiplier);
            contrary_window.baseSeq = ack - 1;
            contrary_window.maxSeq = ack;
            contrary_window.windowMultiplier = 1;
        }else{                                   /*ACK ACK ACK*/
            window.baseSeq = seq - 1;
            window.maxSeq = seq;
            window.windowMultiplier = qAbs(windowMultiplier);
            contrary_window.baseSeq = ack - 1;
            contrary_window.maxSeq = ack;
            contrary_window.windowMultiplier = 1;
        }
        this->windows.insert(streamIndexPlusOne,window);
        this->windows.insert(-streamIndexPlusOne,contrary_window);
    }else{
        if( (SYN && ACK) || (SYN && !ACK) ){ /*前两次握手*/
            (*this->windows.find(streamIndexPlusOne)).baseSeq = seq;
            (*this->windows.find(streamIndexPlusOne)).maxSeq = seq + 1;
            (*this->windows.find(streamIndexPlusOne)).windowMultiplier = qAbs(windowMultiplier);
            if(SYN && !ACK){ /*重新连接，清理窗口*/
                (*this->windows.find(streamIndexPlusOne)).segmentList.clear();
                (*this->windows.find(streamIndexPlusOne)).outOfOrderSegmentList.clear();
                return true;
            }
        }else{    /*正常数据交互*/
            if(windowMultiplier != -1){  /*Window Scale变化*/
                (*this->windows.find(streamIndexPlusOne)).windowMultiplier = qAbs(windowMultiplier);
                dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_WINDOW_SCALE_UPDATE);
            }
        }
    }
    return false;
}


void StreamTcp::addSegmentToWindow(DissectResultBase *dissectResultBase, qint64 streamIndexPlusOne){
    quint32 seq = dissectResultBase->GetAdditionalVal(TCP_SEQ_VAL);
    //quint32 ack = dissectResultBase->GetAdditionalVal(TCP_ACK_VAL);
    quint32 payloadLen = dissectResultBase->GetAdditionalVal(TCP_PAYLOAD_LEN);
    quint32 maxSeq = this->windows.value(streamIndexPlusOne).maxSeq;
    quint32 nextAck = seq + payloadLen;

    segment_t segment;
    if( nextAck <= maxSeq ){
        dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_RETRANSMISSION);
    }else{
        if( seq <= maxSeq ){  /*将片段附加到窗口*/
            segment.seq = seq;
            segment.len = payloadLen;
            segment.index = dissectResultBase->GetIndex();

            if(!this->windows.value(streamIndexPlusOne).segmentList.isEmpty())
                dissectResultBase->AddAdditional(TCP_PRE_SEGMENT,this->windows.value(streamIndexPlusOne).segmentList.last().index);

            dissectResultBase->AddAdditional(TCP_VALIED_DATA_START_PTR,(void*)(dissectResultBase->GetData() + maxSeq - seq));

            (*this->windows.find(streamIndexPlusOne)).segmentList.append(segment);

            (*this->windows.find(streamIndexPlusOne)).maxSeq = nextAck;

            if( this->addSegmentFromOutOfOrderListToWindow(dissectResultBase,streamIndexPlusOne) > 0 ) /*从失序片段中寻找可以连接的分片*/
                dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_OUT_OF_ORDER);
        }else{   /* seq > maxSeq: May Previous Segment not captured*/
            this->addSegmentToOutOfOrderList(dissectResultBase,streamIndexPlusOne);
        }
    }
}

/*返回重组成功的个数*/
qint32 StreamTcp::addSegmentFromOutOfOrderListToWindow(DissectResultBase *dissectResultBase, qint64 streamIndexPlusOne){
    qint32 continuitySegmentCount = 0;
    QStack<qint32> forRemove;
    QList<DissectResultBase*> *baseList = (QList<DissectResultBase*>*)dissectResultBase->GetAdditionalPtr(DISSECT_RESULT_BASE_LIST);
    quint32 maxSeq = this->windows.value(streamIndexPlusOne).maxSeq;
    quint32 nextAck = 0;
    quint32 seq = 0;
    quint32 payloadLen = 0;
    DissectResultBase *base = nullptr;
    for(qint32 index = 0; index < this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.length(); index++){
        base = baseList->at(this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.at(index).index);
        seq = this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.at(index).seq;
        payloadLen = base->GetAdditionalVal(TCP_PAYLOAD_LEN);
        nextAck = seq + payloadLen;
        if( nextAck <= maxSeq ){
            dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_RETRANSMISSION);
            forRemove.push(index);
        }else{
            if( seq <= maxSeq ){
                segment_t segment = this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.at(index);

                if(!this->windows.value(streamIndexPlusOne).segmentList.isEmpty())
                    base->AddAdditional(TCP_PRE_SEGMENT,this->windows.value(streamIndexPlusOne).segmentList.last().index);

                base->AddAdditional(TCP_VALIED_DATA_START_PTR,(void*)(dissectResultBase->GetData() + maxSeq - seq));

                maxSeq = nextAck;
                (*this->windows.find(streamIndexPlusOne)).maxSeq = nextAck;

                (*this->windows.find(streamIndexPlusOne)).segmentList.append(segment);

                forRemove.push(index);
            }else{
                continue;
            }
        }
    }

    continuitySegmentCount = forRemove.length();
    while (!forRemove.isEmpty()) {
        (*this->windows.find(streamIndexPlusOne)).outOfOrderSegmentList.removeAt(forRemove.pop());
    }
    return continuitySegmentCount;
}

void StreamTcp::addSegmentToOutOfOrderList(DissectResultBase *dissectResultBase,qint64 streamIndexPlusOne){
    quint32 seq = dissectResultBase->GetAdditionalVal(TCP_SEQ_VAL);
    //quint32 ack = dissectResultBase->GetAdditionalVal(TCP_ACK_VAL);
    quint32 payloadLen = dissectResultBase->GetAdditionalVal(TCP_PAYLOAD_LEN);

    quint32 temp_seq = 0;
    quint32 temp_payloadLen = 0;
    qint32 index;

    segment_t segment;
    segment.len = payloadLen;
    segment.seq = seq;
    segment.index = dissectResultBase->GetIndex();
    if(this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.isEmpty()){
        dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_PREVIOUS_SEGMENT_NOT_CAPTURED);
    }else{
        for(index = 0; index < this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.length(); index++){
            temp_seq = this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.at(index).seq;
            temp_payloadLen = this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.at(index).len;
            if(seq >= temp_seq && seq <= temp_seq + temp_payloadLen )
                break;
        }
        if(index == this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.length())
            dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_PREVIOUS_SEGMENT_NOT_CAPTURED);
    }
    (*this->windows.find(streamIndexPlusOne)).outOfOrderSegmentList.append(segment);
}
