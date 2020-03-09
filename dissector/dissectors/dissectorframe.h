#ifndef DISSECTORFRAME_H
#define DISSECTORFRAME_H
#include "../global/global.h"
#include "../protree/protree.h"
#include "../info/info.h"
#include "../info/infoeth.h"
#include "../dissres/dissreseth.h"

class DissectorFrame
{
public:
    static ProTree* Dissect(raw_t *raw,dissResList_t *dissResList,qint64 index,Info *info);

private:
    static quint32 flags;  //用作标志位，控制显示内容和检验等操作
    /*
     * bit:
     * 0 :   show the number of bits in the frame
     */

    static QString MsgTop(raw *raw,qint64 index ,Info *info);
    static QString MsgIfId(Info *info);
    static QString MsgIfName(Info *info);
    static QString MsgEncapType(Info *info);
    static QString MsgStrTime(raw_t *raw);
    static QString MsgEpochTime(raw_t *raw);
    static QString MsgDeltaPreCapTime(dissResList_t *dissResList,qint64 index);
    static QString MsgDeltaPreDisTime(dissResList_t *dissResList,qint64 index);
    static QString MsgSinceFirstTime(DissRes *dissRes);
    static QString MsgFrameNo(qint64 index);
    static QString MsgFrameLen(raw_t *raw);
    static QString MsgCapLen(raw_t *raw);
    static QString MsgProsInFrame(DissRes *dissRes);

    static void FlagSetShowBits(uchar option);
    static uchar FlagGetShowBits();
    static void FlagSetEpochTime(uchar option);
    static uchar FlagGetEpochTime();
};

#endif // DISSECTORFRAME_H
