#include "streamtcp.h"
#include <QDebug>

StreamTcp::StreamTcp()
{

}

/*
 * 应该解析出来的结果
 * 1.有效数据开始的位置(ptr)
 * 2.前一个报文的索引
 * 3.Tcp报文的状态
 */
qint64 StreamTcp::AddWithWindow(DissectResultBase *dissectResultBase, quint8 *srcAddr, quint8 *dstAddr, qint32 addr_size, quint8 *srcPort, quint8 *dstPort, qint32 port_size){
    qint64 streamIndexPlusOne =  this->Add(dissectResultBase,srcAddr,dstAddr,addr_size,srcPort,dstPort,port_size);

    quint32 seq = dissectResultBase->GetAdditionalVal(TCP_SEQ_VAL);
    quint32 len = dissectResultBase->GetAdditionalVal(TCP_PAYLOAD_LEN);
    quint32 ack = dissectResultBase->GetAdditionalVal(TCP_ACK_VAL);
    quint64 index = dissectResultBase->GetIndex();

    qint16 window_multiplier = dissectResultBase->GetAdditionalVal(TCP_WINDOW_MULTIPLIER);

    const quint8 *data = NULL;

    struct window_t window;
    if(!this->windows.contains(streamIndexPlusOne))
        window.windowMultiplier = (window_multiplier == 0 ? 1:qAbs(window_multiplier));
    else
        if(window_multiplier != -1)
            (*this->windows.find(streamIndexPlusOne)).windowMultiplier = (window_multiplier == 0 ? 1 : window_multiplier);


    if(!this->windows.contains(streamIndexPlusOne)){
        //qDebug() << "insert base seq" << seq;
        if(dissectResultBase->GetAdditionalVal(TCP_ISSYN)){
            window.baseSeq = seq;
            window.maxSeq = seq + 1;
            this->windows.insert(streamIndexPlusOne,window);
            return streamIndexPlusOne;
        }else{
            window.baseSeq = seq - 1;
            window.maxSeq = seq;
            this->windows.insert(streamIndexPlusOne,window);

            goto  deal_window;
        }
    }else{

        if(dissectResultBase->GetAdditionalVal(TCP_ISSYN)){
            (*this->windows.find(streamIndexPlusOne)).segmentList.clear();
            (*this->windows.find(streamIndexPlusOne)).baseSeq = seq;
            (*this->windows.find(streamIndexPlusOne)).maxSeq = seq + 1;
            this->acks.clear();
            return streamIndexPlusOne;
        }

        deal_window:

        quint32 maxSeq = this->windows.value(streamIndexPlusOne).maxSeq;

        if(this->acks.contains(ack)){
            (*this->acks.find(ack)).append(index);
        }else{
            QList<quint64> packetList;
            packetList.append(index);
            this->acks.insert(ack,packetList);
        }

        //qDebug() << "len = " << len;
        //len = 0;
        if(len == 0)
            return streamIndexPlusOne;

        if( seq + len <= maxSeq ){
            dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_RETRANSMISSION);
        }else{
            if(seq <= maxSeq){
                struct segment_t segment;
                segment.len = len;
                segment.seq = seq;
                segment.index = index;
                data = dissectResultBase->GetProtocolHeaderEndPtrByName("tcp");
                if(this->acks.contains(-streamIndexPlusOne) && this->acks.value(-streamIndexPlusOne).length() >= 4)
                     dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_FAST_RETRANSMISSION);
                if(seq < maxSeq){
                    dissectResultBase->AddAdditional(TCP_VALIED_DATA_START_PTR,(void*)(data + maxSeq - seq));
                }else{
                    dissectResultBase->AddAdditional(TCP_VALIED_DATA_START_PTR,(void*)data);
                }
                if(this->windows.value(streamIndexPlusOne).segmentList.length() > 0)
                    dissectResultBase->AddAdditional(TCP_PRE_SEGMENT,this->windows.value(streamIndexPlusOne).segmentList.last().index);
                else
                    dissectResultBase->AddAdditional(TCP_PRE_SEGMENT,-1);
                (*this->windows.find(streamIndexPlusOne)).segmentList.append(segment);
                (*this->windows.find(streamIndexPlusOne)).maxSeq = seq + len;
                maxSeq = seq + len;

                QStack<qint32> waitforRemove;
                QList<DissectResultBase*> *baseList = (QList<DissectResultBase*>*)dissectResultBase->GetAdditionalPtr(DISSECT_RESULT_BASE_LIST);
                for(qint32 i = 0; i < this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.length(); i++){
                    struct segment_t out_of_order_segment = this->windows.value(streamIndexPlusOne).outOfOrderSegmentList.at(i);
                    if(out_of_order_segment.seq + out_of_order_segment.len <= maxSeq){
                        waitforRemove.push(i);
                    }else{
                        DissectResultBase *base = baseList->at(out_of_order_segment.index);
                        data = base->GetProtocolHeaderEndPtrByName("tcp");
                        if(out_of_order_segment.seq <= maxSeq){
                            if(out_of_order_segment.seq < maxSeq){
                                base->AddAdditional(TCP_VALIED_DATA_START_PTR,(void*)(data + maxSeq - out_of_order_segment.seq));
                            }else{
                                base->AddAdditional(TCP_VALIED_DATA_START_PTR,(void*)data);
                            }
                            base->AddAdditional(TCP_PRE_SEGMENT,this->windows.value(streamIndexPlusOne).segmentList.last().index);
                            (*this->windows.find(streamIndexPlusOne)).segmentList.append(out_of_order_segment);
                            maxSeq = out_of_order_segment.seq + out_of_order_segment.len;
                            (*this->windows.find(streamIndexPlusOne)).maxSeq = maxSeq;
                            waitforRemove.push(i);
                        }else{
                            continue;
                        }
                    }
                }
                while(!waitforRemove.isEmpty()){
                    (*this->windows.find(streamIndexPlusOne)).outOfOrderSegmentList.removeAt(waitforRemove.pop());
                }
            }else{
                struct segment_t outOfOrderSegment;
                outOfOrderSegment.len = len;
                outOfOrderSegment.seq = seq;
                outOfOrderSegment.index = dissectResultBase->GetIndex();
                dissectResultBase->AddAdditional(TCP_STATUS,TCP_A_OUT_OF_ORDER);
                (*this->windows.find(streamIndexPlusOne)).outOfOrderSegmentList.append(outOfOrderSegment);
            }
        }
    }
    return streamIndexPlusOne;
}

quint32 StreamTcp::GetBaseSeq(qint64 streamIndexPlusOne){
    return (*this->windows.find(streamIndexPlusOne)).baseSeq;
}

quint16 StreamTcp::GetWindowMultiplier(qint64 streamIndexPlusOne){
    return this->windows.value(streamIndexPlusOne).windowMultiplier;
}
