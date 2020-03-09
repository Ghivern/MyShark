#ifndef DISSECTORARP_H
#define DISSECTORARP_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../../global/global.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"

class DissectorArp
{
public:
    DissectorArp();
    static void Dissect(raw_t *raw, DissRes *dissRes, ProTree *proTree ,Info *info);



private:
    static quint32 flags;

    static arp_hdr* GetArpHdr(uchar *packet,DissRes *dissRes);
    static QString GetStrAprHType(arp_hdr *header);
    static ushort GetArpHType(arp_hdr *header);
    static QString GetStrArpPType(arp_hdr *header);
    static ushort GetArpPType(arp_hdr *header);
    static uchar GetArpHLength(arp_hdr *header);
    static uchar GetArpPLength(arp_hdr *header);
    static ushort GetArpOpType(arp_hdr *header);
    static QString GetStrArpOpType(arp_hdr *header);
    static QString GetStrArpHSrcAddress(arp_hdr *header);
    static QString GetStrArpPSrcAddress(arp_hdr *header);
    static QString GetStrArpHDstAddress(arp_hdr *header);
    static QString GetStrArpPDstAddress(arp_hdr *header);

    static QString MsgArpInfo(arp_hdr *header);

};

#endif // DISSECTORARP_H
