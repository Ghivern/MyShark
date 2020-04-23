#include "frame.h"
#include <QDebug>

Frame::Frame(ProTree *proTree,DissectResultFrame *dissectResultFrame,void *reserves)
{
        /*Frame处理代码*/
        Q_UNUSED(dissectResultFrame)
        proTree->AddItem("frame","frame summery"); //-
        proTree->AddItem("frame","1.1",1);  // - -
        proTree->AddItem("frame","2.1",1); // - - -
        proTree->Pop(2);
}


