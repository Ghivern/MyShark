#ifndef DISSECTRESULT_H
#define DISSECTRESULT_H

#include "./dissectresultbase.h"

class DissectResult
{
public:
    DissectResult(DissectResultBase *dissectResultBase);
    void SetStremIndexPlusOne(qint64 streamIndexPlusOne);
    void SetNextLayer(void *nextlayer = nullptr);

    qint64 GetStreamIndex();
    qint64 GetStreamIndexPlusOne();
    bool ContainNextLayer();
    void* GetNextLayer();
    DissectResultBase* GetDissectResultBase();


private:
    qint64 streamIndexPlusOne;
    void *nextLayer;
    DissectResultBase *dissectResultBase;

};

#endif // DISSECTRESULT_H
