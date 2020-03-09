#include "dissectorbase.h"

DissectorBase::DissectorBase()
{

}

ProTree* DissectorBase::Dissect(rawList_t *rawList,dissResList_t *dissResList,qint64 index,Info *info){
    Q_UNUSED(rawList)
    Q_UNUSED(dissResList)
    Q_UNUSED(info)
    Q_UNUSED(index)
    return NULL;
}

