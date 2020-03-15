#include "dissectorbase.h"

DissectorBase::DissectorBase()
{

}

ProTree* DissectorBase::Dissect(DissResList *dissResList,qint64 index,Info *info){
    Q_UNUSED(dissResList)
    Q_UNUSED(info)
    Q_UNUSED(index)
    return NULL;
}

