#ifndef DISSECTORETH_H
#define DISSECTORETH_H
#include "../dissectorbase.h"
#include "../dissectorframe.h"
#include "eth_header.h"
class DissectorEth:public DissectorBase
{
public:
    DissectorEth();
    ProTree * Dissect(rawList_t *rawList, dissResList_t *dissResList, qint64 index, Info *info = NULL) override;

    eth_hdr* GetHdr(uchar *packet,DissRes *dissRes);
    ushort GetPtoType(eth_hdr *header);

    QString MsgTop(DissRes *dissRes);

private:
    static quint32 flags;
};

#endif // DISSECTORETH_H
