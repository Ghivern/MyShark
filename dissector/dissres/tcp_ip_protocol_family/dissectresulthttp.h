#ifndef DISSECTRESULTHTTP_H
#define DISSECTRESULTHTTP_H

#include "../dissectresultbase.h"

class DissectResultHttp
{
public:
    DissectResultHttp(DissectResultBase *dissectResultBase,void *reserves = nullptr);

    void* GetNextLayer();
    DissectResultBase* GetDissectResultBase();

private:
    DissectResultBase *dissectResultBase;

    void *nextLayer;
};

#endif // DISSECTRESULTHTTP_H
