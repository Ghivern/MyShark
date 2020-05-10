#include "streamtcp2.h"
#include <QtDebug>

StreamTcp2::StreamTcp2()
    :Stream(4,2)
{

}

void StreamTcp2::ClearWindow(qint64 streamIndexPlusOne){
    (*this->allWindow.find(streamIndexPlusOne)).windowVal = 0;
    (*this->allWindow.find(streamIndexPlusOne)).windowScale = 0;

    (*this->allWindow.find(streamIndexPlusOne)).flags = 0;

    (*this->allWindow.find(streamIndexPlusOne)).baseSeq = 0;
    (*this->allWindow.find(streamIndexPlusOne)).nextSeq = 0;
    (*this->allWindow.find(streamIndexPlusOne)).nextSeqTime.tv_sec = 0;
    (*this->allWindow.find(streamIndexPlusOne)).nextSeqTime.tv_usec = 0;
    (*this->allWindow.find(streamIndexPlusOne)).maxSeqToBeAcked = 0;

    (*this->allWindow.find(streamIndexPlusOne)).lastAck = 0;
    (*this->allWindow.find(streamIndexPlusOne)).lastAckTime.tv_sec = 0;
    (*this->allWindow.find(streamIndexPlusOne)).lastAckTime.tv_usec = 0;

    (*this->allWindow.find(streamIndexPlusOne)).first_rtt.tv_sec = 0;
    (*this->allWindow.find(streamIndexPlusOne)).first_rtt.tv_usec = 0;

    (*this->allWindow.find(streamIndexPlusOne)).haveFirstRrt = false;

    (*this->allWindow.find(streamIndexPlusOne)).lastSegmentFlags = 0;
    (*this->allWindow.find(streamIndexPlusOne)).dupAckNum = 0;
}

qint64 StreamTcp2::AddWithWindow(DissectResultBase *dissectResultBase, quint8 *srcAddr, quint8 *dstAddr, qint32 addr_size, quint8 *srcPort, quint8 *dstPort, qint32 port_size){
    if(dissectResultBase->GetIndex() == 0 && this->allWindow.count() > 0){
        this->allWindow.clear();
        this->ClearStream();
    }

    qint64 streamIndexPlusOne =  this->Add(dissectResultBase,srcAddr,dstAddr,addr_size,srcPort,dstPort,port_size);

//    bool SYN = dissectResultBase->GetAdditionalVal(TCP_ISSYN) == 1 ? true:false;
//    bool FIN = dissectResultBase->GetAdditionalVal(TCP_ISFIN) == 1 ? true:false;
//    bool RST = dissectResultBase->GetAdditionalVal(TCP_ISRST) == 1 ? true:false;
//    bool NOT_ACK_OR_FIN_OR_RST = !(SYN || FIN || RST);
//    bool ACK = dissectResultBase->GetAdditionalVal(TCP_ISACK) == 1 ? true:false;

//    quint32 seq = dissectResultBase->GetAdditionalVal(TCP_SEQ_VAL);
//    quint32 ack = dissectResultBase->GetAdditionalVal(TCP_ACK_VAL);
//    quint32 segLen = dissectResultBase->GetAdditionalVal(TCP_PAYLOAD_LEN);
//    qint32 windowVal = dissectResultBase->GetAdditionalVal(TCP_WINDOW);
//    qint32 windowScale = dissectResultBase->GetAdditionalVal(TCP_WINDOW_SCALE);

    //TcpInfo &tcpInfo = dissectResultBase->GetAdditional(TCP_INFO);
    TcpInfo *tcpInfoPtr = (TcpInfo*)dissectResultBase->GetAdditionalPtr(TCP_INFO_PTR);
    bool SYN = tcpInfoPtr->SYN;
    bool FIN = tcpInfoPtr->FIN;
    bool RST = tcpInfoPtr->RST;
    bool ACK = tcpInfoPtr->ACK;
    bool NOT_ACK_OR_FIN_OR_RST = !(SYN || FIN || RST);

    quint32 seq = tcpInfoPtr->seq;
    quint32 ack = tcpInfoPtr->ack;
    quint32 segLen = tcpInfoPtr->segLen;
    qint32 windowVal = tcpInfoPtr->windowVal;
    qint32 windowScale = tcpInfoPtr->windowSclae;


    if( !allWindow.contains(streamIndexPlusOne) ){
        Window aWindow;
        Window aContraryWindow;
        this->allWindow.insert(streamIndexPlusOne,aWindow);
        this->allWindow.insert(-streamIndexPlusOne,aContraryWindow);
    }

    /*处理baseSeq*/
//    if( ( this->allWindow.value(streamIndexPlusOne).flags & TCP_S_BASE_SEQ_SET ) && SYN && !ACK ){
//        this->ClearWindow(streamIndexPlusOne);
//        this->ClearWindow(-streamIndexPlusOne);
//    }

    if( !( this->allWindow.value(streamIndexPlusOne).flags & TCP_S_BASE_SEQ_SET ) ){
        if( SYN ){
            (*this->allWindow.find(streamIndexPlusOne)).baseSeq = seq;
            (*this->allWindow.find(streamIndexPlusOne)).flags |= ACK ? TCP_S_SAW_SYNACK : TCP_S_SAW_SYN;

            if( SYN && !ACK ){
                (*this->allWindow.find(streamIndexPlusOne)).first_rtt.tv_sec = tcpInfoPtr->time.tv_sec;
                (*this->allWindow.find(streamIndexPlusOne)).first_rtt.tv_usec = tcpInfoPtr->time.tv_usec;
            }

            if( SYN && ACK && this->allWindow.value(-streamIndexPlusOne).first_rtt.tv_sec != 0){
                (*this->allWindow.find(streamIndexPlusOne)).first_rtt.tv_sec = tcpInfoPtr->time.tv_sec -  this->allWindow.value(-streamIndexPlusOne).first_rtt.tv_sec;
                (*this->allWindow.find(streamIndexPlusOne)).first_rtt.tv_usec = tcpInfoPtr->time.tv_usec -  this->allWindow.value(-streamIndexPlusOne).first_rtt.tv_usec;

                (*this->allWindow.find(-streamIndexPlusOne)).first_rtt.tv_sec = this->allWindow.value(streamIndexPlusOne).first_rtt.tv_sec;
                (*this->allWindow.find(-streamIndexPlusOne)).first_rtt.tv_usec = this->allWindow.value(streamIndexPlusOne).first_rtt.tv_usec;

                (*this->allWindow.find(streamIndexPlusOne)).haveFirstRrt = true;
                (*this->allWindow.find(-streamIndexPlusOne)).haveFirstRrt = true;
            }

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
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_ZERO_WINDOW_PROBE);
        tcpInfoPtr->status |= TCP_A_ZERO_WINDOW_PROBE;
        goto finished;
    }

    /*TCP_A_ZERO_WINDOW*/
    if( windowVal == 0 && NOT_ACK_OR_FIN_OR_RST ){
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_ZERO_WINDOW);
        tcpInfoPtr->status |= TCP_A_ZERO_WINDOW;
    }

    /*TCP_A_LOST_PACKET*/
    if( this->allWindow.value(streamIndexPlusOne).nextSeq
            && seq > this->allWindow.value(streamIndexPlusOne).nextSeq
            && !RST ){
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_LOST_PACKET);
        tcpInfoPtr->status |= TCP_A_LOST_PACKET;
    }

    /*TCP_A_KEEP_ALIVE*/
    if( ( segLen == 0 || segLen == 1)
            && seq == this->allWindow.value(streamIndexPlusOne).nextSeq - 1
            && NOT_ACK_OR_FIN_OR_RST){
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_KEEP_ALIVE);
        tcpInfoPtr->status |= TCP_A_KEEP_ALIVE;
    }

    /*TCP_A_WINDOW_UPDATE*/
    if( segLen == 0
            && windowVal
            && windowVal != this->allWindow.value(streamIndexPlusOne).windowVal
            && seq == this->allWindow.value(streamIndexPlusOne).nextSeq
            && ack == this->allWindow.value(streamIndexPlusOne).lastAck
            && NOT_ACK_OR_FIN_OR_RST){
       //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_WINDOW_UPDATE);
       tcpInfoPtr->status |= TCP_A_WINDOW_UPDATE;
    }

    /*TCP_A_WINDOW_FULL*/
    if( segLen > 0
            && this->allWindow.value(streamIndexPlusOne).windowScale
            && (seq + segLen) == this->allWindow.value(streamIndexPlusOne).lastAck +(this->allWindow.value(streamIndexPlusOne).windowVal << (this->allWindow.value(streamIndexPlusOne).windowScale == -1 ? 0 : this->allWindow.value(streamIndexPlusOne).windowScale))
            && NOT_ACK_OR_FIN_OR_RST
            ){
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_WINDOW_FULL);
        tcpInfoPtr->status |= TCP_A_WINDOW_FULL;
    }

    /*TCP_A_KEEP_ALIVE_ACK*/
    if( segLen == 0
            && this->allWindow.value(streamIndexPlusOne).windowVal
            && windowVal == this->allWindow.value(streamIndexPlusOne).windowVal
            && (this->allWindow.value(-streamIndexPlusOne).lastSegmentFlags & TCP_A_KEEP_ALIVE)){
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_KEEP_ALIVE_ACK);
        tcpInfoPtr->status |= TCP_A_KEEP_ALIVE_ACK;
        goto finished;
    }

    /*TCP_A_ZERO_WINDOW_PROBE_ACK*/
    if ( segLen == 0
         && seq == this->allWindow.value(streamIndexPlusOne).nextSeq
         && ack == this->allWindow.value(streamIndexPlusOne).lastAck
         && (this->allWindow.value(-streamIndexPlusOne).lastSegmentFlags & TCP_A_ZERO_WINDOW_PROBE)
         && NOT_ACK_OR_FIN_OR_RST){
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_ZERO_WINDOW_PROBE_ACK);
        tcpInfoPtr->status |= TCP_A_ZERO_WINDOW_PROBE_ACK;
        goto finished;
    }

    /*TCP_A_DUPLICATE_ACK*/
    if( segLen == 0
            && windowVal
            && windowVal == this->allWindow.value(streamIndexPlusOne).windowVal
            && ack == this->allWindow.value(streamIndexPlusOne).lastAck
            && NOT_ACK_OR_FIN_OR_RST){
        (*this->allWindow.find(streamIndexPlusOne)).dupAckNum++;
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_DUPLICATE_ACK);
        tcpInfoPtr->status |= TCP_A_DUPLICATE_ACK;
        //dissectResultBase->AddAdditional(TCP_DUPACK_NUM,this->allWindow.value(streamIndexPlusOne).dupAckNum);
        //dissectResultBase->AddAdditional(TCP_DUPACK_FRAME,this->allWindow.value(streamIndexPlusOne).lastNonDupAck);

        tcpInfoPtr->dupack_num = this->allWindow.value(streamIndexPlusOne).dupAckNum;
        tcpInfoPtr->dupack_frame = this->allWindow.value(streamIndexPlusOne).lastNonDupAck;
    }

    finished:

    if( ack != this->allWindow.value(streamIndexPlusOne).lastAck ){
        (*this->allWindow.find(streamIndexPlusOne)).lastNonDupAck = dissectResultBase->GetIndex();
        (*this->allWindow.find(streamIndexPlusOne)).dupAckNum = 0;
    }

    /*TCP_A_ACK_LOST_PACKET*/
    if( this->allWindow.value(-streamIndexPlusOne).maxSeqToBeAcked
            && ack > this->allWindow.value(-streamIndexPlusOne).maxSeqToBeAcked
            && ACK){
        //dissectResultBase->OrToAddition(TCP_STATUS2,TCP_A_ACK_LOST_PACKET);
        tcpInfoPtr->status |= TCP_A_ACK_LOST_PACKET;
        (*this->allWindow.find(-streamIndexPlusOne)).maxSeqToBeAcked = this->allWindow.value(-streamIndexPlusOne).nextSeq;
    }

    if( tcpInfoPtr->segLen > 0 || tcpInfoPtr->SYN || tcpInfoPtr->FIN ){
        bool seq_not_advanced = this->allWindow.value(streamIndexPlusOne).nextSeq
                && tcpInfoPtr->seq < this->allWindow.value(streamIndexPlusOne).nextSeq;

        quint64 t;
        quint64 ooo_thres;

        if( tcpInfoPtr->status & TCP_A_KEEP_ALIVE )
            goto finished_checking_retransmission_type;

        if( tcpInfoPtr->segLen > 1 && this->allWindow.value(streamIndexPlusOne).nextSeq - 1 == seq )
            seq_not_advanced = false;

        /*TCP_A_Fast_Retransmission*/
        t = (tcpInfoPtr->time.tv_sec - this->allWindow.value(-streamIndexPlusOne).lastAckTime.tv_sec) * 1000000000;
        t = t + tcpInfoPtr->time.tv_usec - this->allWindow.value(-streamIndexPlusOne).lastAckTime.tv_usec;
        if( seq_not_advanced
                && this->allWindow.value(-streamIndexPlusOne).dupAckNum >= 3
                && this->allWindow.value(-streamIndexPlusOne).lastAck == seq
                && t < 2000000000){
            tcpInfoPtr->status |= TCP_A_FAST_RETRANSMISSION;
            goto finished_checking_retransmission_type;
        }

        /*TCP_A_OUT_OF_ORDER*/
        t = ( tcpInfoPtr->time.tv_sec - this->allWindow.value(streamIndexPlusOne).nextSeqTime.tv_sec ) * 1000000000;
        t = t + tcpInfoPtr->time.tv_usec - this->allWindow.value(streamIndexPlusOne).nextSeqTime.tv_usec;
        if( !this->allWindow.value(streamIndexPlusOne).haveFirstRrt ){
            ooo_thres = 3000000;
        }else{
            ooo_thres = this->allWindow.value(streamIndexPlusOne).first_rtt.tv_usec
                    + this->allWindow.value(streamIndexPlusOne).first_rtt.tv_sec * 1000000000;
            qDebug() << "sec" << this->allWindow.value(streamIndexPlusOne).first_rtt.tv_sec;
            qDebug() << "usec" << this->allWindow.value(streamIndexPlusOne).first_rtt.tv_usec;
        }
        qDebug() << "ooo_thres" << ooo_thres;
        if( seq_not_advanced
                && t < ooo_thres
                && this->allWindow.value(streamIndexPlusOne).nextSeq != tcpInfoPtr->seq + tcpInfoPtr->segLen){
            tcpInfoPtr->status |= TCP_A_OUT_OF_ORDER;
            goto finished_checking_retransmission_type;
        }

        /*TCP_A_SPURIOUS_RETRANSMISSION*/
        if( tcpInfoPtr->segLen > 0
                && this->allWindow.value(-streamIndexPlusOne).lastAck
                && tcpInfoPtr->seq + tcpInfoPtr->segLen < this->allWindow.value(-streamIndexPlusOne).lastAck){
            tcpInfoPtr->status |= TCP_A_SPURIOUS_RETRANSMISSION;
            goto finished_checking_retransmission_type;
        }

        /*TCP_A_RETRANSMISSION*/
        if( seq_not_advanced ){
            tcpInfoPtr->status |= TCP_A_RETRANSMISSION;
        }

    }

    finished_checking_retransmission_type:

    /*更新数据*/
    quint32 nextseq = seq + segLen;
    if( SYN || FIN ){
        nextseq += 1;
    }
    //quint32 segmentFlag = dissectResultBase->GetAdditionalVal(TCP_STATUS2);
    if( nextseq > this->allWindow.value(streamIndexPlusOne).nextSeq
            || !this->allWindow.value(streamIndexPlusOne).nextSeq ){
        if( !( tcpInfoPtr->status & TCP_A_ZERO_WINDOW_PROBE) ){
            (*this->allWindow.find(streamIndexPlusOne)).nextSeq = nextseq;
            (*this->allWindow.find(streamIndexPlusOne)).nextSeqTime.tv_sec = tcpInfoPtr->time.tv_sec;
            (*this->allWindow.find(streamIndexPlusOne)).nextSeqTime.tv_usec = tcpInfoPtr->time.tv_usec;
        }
    }

    if( seq == this->allWindow.value(streamIndexPlusOne).maxSeqToBeAcked
            || !this->allWindow.value(streamIndexPlusOne).maxSeqToBeAcked){
        if( !( tcpInfoPtr->status & TCP_A_ZERO_WINDOW_PROBE) ){
            (*this->allWindow.find(streamIndexPlusOne)).maxSeqToBeAcked = this->allWindow.value(streamIndexPlusOne).nextSeq;
        }
    }

    (*this->allWindow.find(streamIndexPlusOne)).windowScale = windowScale;
    (*this->allWindow.find(streamIndexPlusOne)).windowVal = windowVal;
    (*this->allWindow.find(streamIndexPlusOne)).lastAckTime.tv_sec = tcpInfoPtr->time.tv_sec;
    (*this->allWindow.find(streamIndexPlusOne)).lastAckTime.tv_usec = tcpInfoPtr->time.tv_usec;
    (*this->allWindow.find(streamIndexPlusOne)).lastAck = ack;
    (*this->allWindow.find(streamIndexPlusOne)).lastSegmentFlags = tcpInfoPtr->status;

    return streamIndexPlusOne;
}

quint32 StreamTcp2::GetBaseSeq(qint64 streamIndexPlusOne){
    if( !this->allWindow.contains(streamIndexPlusOne) )
        return 0;
    return this->allWindow.value(streamIndexPlusOne).baseSeq;
}

quint32 StreamTcp2::GetWindowMultiplier(qint64 streamIndexPlusOne){
    if( this->allWindow.value(streamIndexPlusOne).windowScale == -1){
        return 1;
    }
    return qPow(2,this->allWindow.value(streamIndexPlusOne).windowScale);
}

void StreamTcp2::Clear(){
    this->ClearStream();
    this->allWindow.clear();
}
