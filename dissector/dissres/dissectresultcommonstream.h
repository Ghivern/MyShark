#ifndef DISSECTRESULTCOMMONSTREAM_H
#define DISSECTRESULTCOMMONSTREAM_H

#include <QtCore>
#include "math.h"

#include "../stream/stream.h"

class DissectResultCommonStream
{
public:
    DissectResultCommonStream();

    qint64 GetOriginalStreamIndex();
    qint64 GetStreamIndex();

protected:
    qint64 streamIndexPlusOne;
};

#endif // DISSECTRESULTCOMMONSTREAM_H
