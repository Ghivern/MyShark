#include "frame.h"
#include <QDebug>


Frame::Frame(ProTree *proTree,DissectResultFrame *dissectResultFrame,void *reserves)
{
    Q_UNUSED(reserves)

    quint64 option = DissectResultBase::GetDissectorOptionPtr()->value("frame");
    qint32 interfaceId = DissectResultBase::GetInterfaceId();
    QString interfaceName(DissectResultBase::GetInterfaceName());
    qint32 linklayer = DissectResultBase::GetLinklayerType();
    QString linklayerName(DissectResultBase::GetLinklayerTypeName());


    /*Summery*/
    proTree->AddItem("frame",
                     QString("Frame %1: %2 bytes on wire%3, %4 bytes captured%5 on interface %6")
                     .arg(dissectResultFrame->GetDissectResultBase()->GetIndex())
                     .arg(dissectResultFrame->GetLen())
                     .arg(
                         option & FRAME_SHOW_NUMBER_OF_BITS? QString::asprintf(" (%d bits)",dissectResultFrame->GetLen() * 8) : ""
                         )
                     .arg(dissectResultFrame->GetCapLen())
                     .arg(
                         option & FRAME_SHOW_NUMBER_OF_BITS? QString::asprintf(" (%d bits)",dissectResultFrame->GetCapLen() * 8) : ""
                         )
                     .arg(interfaceId)
                     ,-2
                     ,-2
                     );

    /*Summery - Interface*/
    proTree->AddItem("frame",
                     QString("Interface Id: %1 ( %2 )")
                     .arg(interfaceId)
                     .arg(interfaceName)
                     ,ProTree::level::NEW
                     );


    /*Summery - Interface - Interface Name*/
    proTree->AddItem("frame",
                     QString("Interface Name: %1")
                     .arg(interfaceName)
                     ,ProTree::level::NEW
                     );

    /*Summery - Encapsulation*/
    proTree->Pop();
    proTree->AddItem("frame",
                     QString("Encapsulation type: %1 (%2)")
                     .arg(linklayerName)
                     .arg(linklayer)
                     );

    /*Summery - Arrival Time*/
    QDateTime dt = QDateTime::fromSecsSinceEpoch(dissectResultFrame->GetCaptureTimeSec());
    proTree->AddItem("frame",
                     QString("Arrival Time: %1.%2 ( %3 )")
                     .arg(dt.toString("MMMM d, yyyy hh:mm:ss"))
                     .arg(dissectResultFrame->GetCaptureTimeUSec())
                     .arg(dt.timeZone().displayName(dt,QTimeZone::NameType::DefaultName))
                     );

    /*Summery - Epoch Time*/
    if( option & FRAME_GENERATE_EPOCH_TIME ){
        proTree->AddItem("frame",
                         QString("Epoch Time: %1 seconds")
                         .arg(QString::asprintf("%.9f",dissectResultFrame->GetCaptureTimeSec()
                                                + dissectResultFrame->GetCaptureTimeUSec()/1000000000.0)
                         )
                         );
    }

    /*Summery - Delta from previous captured frame*/
    QList<DissectResultBase*> *baseList  = (QList<DissectResultBase*>*)dissectResultFrame->GetDissectResultBase()->GetAdditionalPtr(DISSECT_RESULT_BASE_LIST);
    DissectResultBase *preBase = nullptr;
    if( dissectResultFrame->GetDissectResultBase()->GetIndex() == 0)
        preBase = dissectResultFrame->GetDissectResultBase();
    else
        preBase = baseList->at(dissectResultFrame->GetDissectResultBase()->GetIndex() - 1);
    proTree->AddItem("frame",
                     QString("[Time delta from previous captured frame: %1 seconds]")
                     .arg(QString::asprintf("%.9f",
                            (dissectResultFrame->GetDissectResultBase()->GetPkthdr()->ts.tv_sec
                             - preBase->GetPkthdr()->ts.tv_sec)
                             + (dissectResultFrame->GetDissectResultBase()->GetPkthdr()->ts.tv_usec
                             - preBase->GetPkthdr()->ts.tv_usec)/1000000000.0)
                     )
                     );

    /*Summery - Time since first frame*/
    proTree->AddItem("frame",
                     QString("[Time since first frame: %1 seconds]")
                     .arg(
                         QString::asprintf("%.9f",dissectResultFrame->GetRelativeTimeSinceFirstPacket())
                     )
                     );

    /*Summery - Frame Number*/
    proTree->AddItem("frame",
                     QString("Frame Number: %1")
                     .arg(dissectResultFrame->GetDissectResultBase()->GetIndex())
                     );

    /*Summery - Frame Length*/
    proTree->AddItem("frame",
                     QString("Frame Length: %1 bytes%2")
                     .arg(dissectResultFrame->GetLen())
                     .arg((option & FRAME_SHOW_NUMBER_OF_BITS ?
                             QString::asprintf(" (%d bits)",dissectResultFrame->GetLen() * 8)
                             :"")
                     )
                     );

    /*Summery - Capture Length*/
    proTree->AddItem("frame",
                     QString("Capture Length: %1 bytes%2")
                     .arg(dissectResultFrame->GetLen())
                     .arg((option & FRAME_SHOW_NUMBER_OF_BITS ?
                             QString::asprintf(" (%d bits)",dissectResultFrame->GetCapLen() * 8)
                             :"")
                     )
                     );

    /*Summery - Frame MD5 Hash*/
    if( option & FRAME_GENERATE_MD5_HASH ){
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData((const char*)dissectResultFrame->GetDissectResultBase()->GetData()
                                   ,dissectResultFrame->GetCapLen());
        QByteArray byteArray = hash.result().toHex();
        QString hashCode = byteArray;
//        for(qint32 index = 0; index < byteArray.length(); index++){
//            hashCode.append(QString::asprintf("%02x",(quint8)byteArray.at(index)));
//        }
//        qDebug() << byteArray.length();
        proTree->AddItem("frame",
                         QString("[Frame MD5 Hash: %1]")
                         .arg(hashCode)
                         );
    }

    /*Summery - Protocols in frame*/
    proTree->AddItem("frame",
                     QString("[Protocols in frame: %1]")
                     .arg(dissectResultFrame->GetDissectResultBase()->GetProtocolsInFrame())
                     );

    /*Summery*/
    proTree->Pop(); // -
}


