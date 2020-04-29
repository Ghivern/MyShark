#include "frame.h"
#include <QDebug>

Frame::Frame(ProTree *proTree,DissectResultFrame *dissectResultFrame,void *reserves)
{
    QList<void*> *r = (QList<void*>*)reserves;
    qint32 interfaceId = *(qint32*)r->at(1);
    QString interfaceName(*(QString*)r->at(2));
    Q_UNUSED(reserves)
        /*Frame处理代码*/
        Q_UNUSED(dissectResultFrame)
//        proTree->AddItem("frame","frame summery"); //-
//        proTree->AddItem("frame","1.1",1);  // - -
//        proTree->AddItem("frame","2.1",1); // - - -
//        proTree->Pop(2);

    proTree->AddItem("frame",
                     QString("Frame %1: %2 bytes on wire (%3 bits), %4 bytes captured (%5 bits) on interface %6")
                     .arg(dissectResultFrame->GetIndex())
                     .arg(dissectResultFrame->GetLen())
                     .arg(dissectResultFrame->GetLen() * 8)
                     .arg(dissectResultFrame->GetCapLen())
                     .arg(dissectResultFrame->GetCapLen() * 8)
                     .arg(interfaceId)
                     ,-2
                     ,-2
                     );  // -
    proTree->AddItem("frame",
                     QString("Interface Id: %1 ( %2 )")
                     .arg(interfaceId)
                     .arg(interfaceName)
                     ,1
                     );  // - -
    proTree->AddItem("frame",
                     QString("Interface Name: %1")
                     .arg(interfaceName)
                     ,1
                     ); // - - -

    proTree->Pop(2); // -
}


