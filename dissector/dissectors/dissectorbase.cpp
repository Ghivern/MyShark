#include "dissectorbase.h"

DissectorBase::DissectorBase()
{

}

ProTree* DissectorBase::Dissect(DissResList_t *dissResList,qint64 index,Info *info){
    Q_UNUSED(dissResList)
    Q_UNUSED(info)
    Q_UNUSED(index)
    return NULL;
}

const quint8* DissectorBase::BaseGetHdr(DissRes *dissRes,QString proName, qint32 len, bool first, bool hide){
    if(first)
        dissRes->AddToProtocolStackWithSE(proName,len,hide);
    return dissRes->GetProStartPtr(proName);

}
