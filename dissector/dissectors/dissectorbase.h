#ifndef DISSECTORBASE_H
#define DISSECTORBASE_H

#include "../dissector_global.h"
#include "../protree/protree.h"
#include "../dissres/dissres.h"
#include "../info/info.h"
#include "../stream/streamrecorder.h"

class DissectorBase
{
public:
    DissectorBase();
    virtual ProTree* Dissect(DissResList_t *dissResList,qint64 index,Info *info = NULL);

    static const quint8* BaseGetHdr(DissRes *dissRes,QString proName,qint32 len,bool first = false, bool hide = false);
};

#endif // DISSECTORBASE_H
