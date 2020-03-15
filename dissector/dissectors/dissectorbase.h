#ifndef DISSECTORBASE_H
#define DISSECTORBASE_H

#include "../dissector_global.h"
#include "../protree/protree.h"
#include "../dissres/dissres.h"
#include "../info/info.h"

class DissectorBase
{
public:
    DissectorBase();
    virtual ProTree* Dissect(DissResList *dissResList,qint64 index,Info *info = NULL);
};

#endif // DISSECTORBASE_H
