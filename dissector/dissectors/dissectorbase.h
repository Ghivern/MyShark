#ifndef DISSECTORBASE_H
#define DISSECTORBASE_H

#include "../dissector_global.h"
#include "../protree/protree.h"
#include "../../global/global.h"
#include "../info/info.h"

class DissectorBase
{
public:
    DissectorBase();
    virtual ProTree* Dissect(rawList_t *rawList,dissResList_t *dissResList,qint64 index,Info *info = NULL);
};

#endif // DISSECTORBASE_H
