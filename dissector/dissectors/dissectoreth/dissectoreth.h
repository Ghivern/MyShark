#ifndef DISSECTORETH_H
#define DISSECTORETH_H
#include "../dissectorbase.h"
#include "../dissectorframe.h"
#include "eth_header.h"
class DissectorEth:public DissectorBase
{
public:
    DissectorEth();
    ProTree * Dissect(DissResList_t *dissResList, qint64 index, Info *info = NULL);

private:
    static quint32 flags;

    eth_hdr* GetHdr(DissRes *packet,bool first=false);
    ushort GetPtoType(eth_hdr *header);
    quint32 GetIntFCS(DissRes *packet);
    QString GetStrFCS(DissRes *packet);
    quint32 GetCalculatedIntFCS(DissRes *packet);
    QString GetCalculatedStrFCS(DissRes *packet);

    static void FlagSetValidateFCSBit(uchar option);
    static uchar FlagGetValidateFCSBit();

    QString MsgTop(DissRes *dissRes);
    QString MsgDst(DissRes *dissRes);
    QString MsgSrc(DissRes *dissRes);
    QString MsgType(eth_hdr *eth);
    QString MsgLG(uchar *mac);
    QString MsgIG(uchar *mac);

    void DealFCS(ProTree *tree,DissRes *dissRes);
};

#endif // DISSECTORETH_H
