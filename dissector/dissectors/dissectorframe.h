#ifndef DISSECTORFRAME_H
#define DISSECTORFRAME_H
#include "../protree/protree.h"
#include "../info/info.h"
#include "../info/infoeth.h"
#include "../dissres/dissreseth.h"

class DissectorFrame
{
public:
    static ProTree* Dissect(DissResList_t *dissResList,qint64 index,Info *info);

private:
    static quint32 flags;  //用作标志位，控制显示内容和检验等操作

    static QString MsgTop(DissRes *dissRes,qint64 index ,Info *info);
    static QString MsgIfId(Info *info);
    static QString MsgIfName(Info *info);
    static QString MsgEncapType(Info *info);
    static QString MsgStrTime(DissRes *packet);
    static QString MsgEpochTime(DissRes *packet);
    static QString MsgDeltaPreCapTime(DissResList_t *dissResList,qint64 index);
    static QString MsgDeltaPreDisTime(DissResList_t *dissResList,qint64 index);
    static QString MsgSinceFirstTime(DissRes *dissRes);
    static QString MsgFrameNo(qint64 index);
    static QString MsgFrameLen(DissRes *packet);
    static QString MsgCapLen(DissRes *packet);
    static QString MsgProsInFrame(DissRes *dissRes);

    //操作flags标志位
    static void FlagSetShowBits(uchar option);
    static uchar FlagGetShowBits();
    static void FlagSetEpochTime(uchar option);
    static uchar FlagGetEpochTime();
};

#endif // DISSECTORFRAME_H
