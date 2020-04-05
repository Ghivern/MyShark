#include "dissectorframe.h"

quint32 DissectorFrame::flags = 0x1;  //赋予默认配置
/*
 * bit:
 * 0:       1    show the number of bits in the frame
 * 1:       0    generate an epoch time entry for each frame
*/

ProTree* DissectorFrame::Dissect(DissResList_t *dissResList,qint64 index,Info *info){
    ProTree *proTree = NULL;
    if(info == NULL){
        qDebug() << "DissectorFrame : info == NULL";
        dissResList->at(index)->SetHeadersLen(0);
        dissResList->at(index)->AddToProtocolStackWithSE("eth",0);

    }else{
        proTree = new ProTree();
        DissResEth *dissResEth = (DissResEth*)(dissResList->at(index));
        proTree->AddItem("eth",DissectorFrame::MsgTop(dissResList->at(index),index,info),0,dissResEth->GetCapLen() - 1); // -
        proTree->AddItem("eth",DissectorFrame::MsgIfId(info),ProTree::level::NEW); // - -
        proTree->AddItem("eth",DissectorFrame::MsgIfName(info),ProTree::level::NEW); // - - -
        proTree->AddItem("eth",DissectorFrame::MsgEncapType(info),-1); // - -
        proTree->AddItem("eth",DissectorFrame::MsgStrTime(dissResList->at(index))); // - -
        proTree->AddItem("eth",DissectorFrame::MsgEpochTime(dissResList->at(index))); // - -
        proTree->AddItem("eth",DissectorFrame::MsgDeltaPreCapTime(dissResList,index)); // - -
        proTree->AddItem("eth",DissectorFrame::MsgDeltaPreDisTime(dissResList,index)); // - -
        proTree->AddItem("eth",DissectorFrame::MsgSinceFirstTime(dissResList->at(index))); // - -
        proTree->AddItem("eth",DissectorFrame::MsgFrameNo(index));  // - -
        proTree->AddItem("eth",DissectorFrame::MsgFrameLen(dissResList->at(index)));  // - -
        proTree->AddItem("eth",DissectorFrame::MsgCapLen(dissResList->at(index)));  // - -
        proTree->AddItem("eth",DissectorFrame::MsgProsInFrame(dissResList->at(index)));  // - -
        proTree->Pop(); // -
    }
    return proTree;
}

//Msg方法
QString DissectorFrame::MsgTop(DissRes *packet, qint64 index ,Info *info){
    uchar bit = DissectorFrame::FlagGetShowBits();
    return QString::asprintf("Frame %lld: %d bytes on wire",index,packet->GetLen())
            + (bit == 0 ? " , ":QString::asprintf(" (%d bits), ",packet->GetLenBit()))
            + QString::asprintf("%d bytes captured",packet->GetCapLen())
            + (bit == 0 ? " ":QString::asprintf(" (%d bits) ",packet->GetCapLenBit()))
            + QString::asprintf("on interface %d",info->GetDevIndex())
            ;
}

QString DissectorFrame::MsgIfId(Info *info){
    return QString::asprintf("Interface id : %d (",info->GetDevIndex())
            +info->GetDevName()
            + ")";
}

QString DissectorFrame::MsgIfName(Info *info){
    return QString("Interface name : ") + info->GetDevName();
}

QString DissectorFrame::MsgEncapType(Info *info){
    return QString("Encapsulation type : ") + info->GetDatalinkName();
}

QString DissectorFrame::MsgStrTime(DissRes *packet){
    time_t time_sec = packet->GetTimeval().tv_sec;
    struct tm *local;
    local = localtime(&time_sec);
    char strTime[128];
    strftime(strTime,64, "%Y-%m-%d %H:%M:%S", local);
    return QString::asprintf("Arrival time : %s.%ld",strTime,packet->GetTimeval().tv_usec);
}

QString DissectorFrame::MsgEpochTime(DissRes *packet){
    return QString::asprintf("Epoch time : %ld.%ld seconds"
                             ,packet->GetTimeval().tv_sec,packet->GetTimeval().tv_usec);
}

QString DissectorFrame::MsgDeltaPreCapTime(DissResList_t *dissResList, qint64 index){
    DissResEth *dissResEth = ((DissResEth*)dissResList->at(index));
    DissResEth *dissResEthP = ((DissResEth*)dissResList->at(index == 0 ? 0:index-1));
    return QString::asprintf("[Time delta from previous captured frame : %f seconds]"
                             ,dissResEth->GetTimeSinceFirstPacket()
                             - dissResEthP->GetTimeSinceFirstPacket()
                             );
}

QString DissectorFrame::MsgDeltaPreDisTime(DissResList_t *dissResList, qint64 index){
    DissResEth *dissResEth = ((DissResEth*)dissResList->at(index));
    DissResEth *dissResEthP = ((DissResEth*)dissResList->at(index == 0 ? 0:index-1));
    return QString::asprintf("[Time delta from previous displayed frame : %f seconds]"
                             ,dissResEth->GetTimeSinceFirstPacket()
                             - dissResEthP->GetTimeSinceFirstPacket()
                             );
}

QString DissectorFrame::MsgSinceFirstTime(DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    return QString::asprintf("[Time since first frame : %f seconds]"
                             ,dissResEth->GetTimeSinceFirstPacket());
}

QString DissectorFrame::MsgFrameNo(qint64 index){
    return QString::asprintf("Frame number : %lld",index);
}

QString DissectorFrame::MsgFrameLen(DissRes *packet){
    return QString::asprintf("Frame length : %d bytes (%d bits)",packet->GetLen(),packet->GetLenBit());
}

QString DissectorFrame::MsgCapLen(DissRes *packet){
    return QString::asprintf("Capure length : %d bytes (%d bits)",packet->GetCapLen(),packet->GetCapLenBit());
}

QString DissectorFrame::MsgProsInFrame(DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    QString pros;
    for(qint32 index = 0; index < dissResEth->GetProtocolStack().length() - 1; index++)
        pros += dissResEth->GetProtocolStack().at(index) + ":";
    pros += dissResEth->GetProtocolStack().back();
    return pros;
}
//Flag方法
void DissectorFrame::FlagSetShowBits(uchar option){
    if(option == 0)
        DissectorFrame::flags &= 0xfffffffe;
    else
        DissectorFrame::flags |= 0x00000001;
}

uchar DissectorFrame::FlagGetShowBits(){
    return (DissectorFrame::flags & 0x00000001) >> 0;
}

void DissectorFrame::FlagSetEpochTime(uchar option){
    if(option == 0)
        DissectorFrame::flags &= 0xfffffffd;
    else
        DissectorFrame::flags |= 0x00000002;
}

uchar DissectorFrame::FlagGetEpochTime(){
    return (DissectorFrame::flags & 0x00000002) >> 1;
}
