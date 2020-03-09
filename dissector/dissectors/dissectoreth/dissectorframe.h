#ifndef DISSECTORFRAME_H
#define DISSECTORFRAME_H
#include "../global/global.h"
#include "../protree/protree.h"
#include "../info/info.h"
#include "../dissres/dissreseth.h"

class DissectorFrame
{
public:
    static ProTree* Dissect(raw_t *raw,dissResList_t *dissResList,qint64 index,Info *info);

private:
    static quint32 flags;  //用作标志位，控制显示内容和检验等操作
};

#endif // DISSECTORFRAME_H
