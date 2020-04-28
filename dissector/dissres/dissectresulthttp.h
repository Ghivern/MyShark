#ifndef DISSECTRESULTHTTP_H
#define DISSECTRESULTHTTP_H

#include "dissectresultbase.h"

class DissectResultHttp
{
public:
    DissectResultHttp(DissectResultBase *dissectResultBase);

    void* GetNextLayer();
    DissectResultBase* GetDissectResultBase();

private:
    DissectResultBase *dissectResultBase;

    void *nextLayer;
};

#endif // DISSECTRESULTHTTP_H
