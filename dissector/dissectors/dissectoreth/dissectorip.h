#ifndef DISSECTORIP_H
#define DISSECTORIP_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../../global/global.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"

class DissectorIp
{
public:
    DissectorIp();

    static void Dissect(raw_t *raw, DissRes *dissRes, ProTree *proTree ,Info *info);

private:
    static quint32 flags;
    /*
     * bit
     * 0:    0: uncheck    1: check
     */

    static ip_hdr *GetIpHdr(uchar *packet,DissRes *dissRes);
    static uchar GetIpHdrLen(ip_hdr *header);
    static uchar GetIpPType(ip_hdr *header);

    static void FlagSetCheck(uchar option);
    static uchar FlagGetCheck();
};

#endif // DISSECTORIP_H
