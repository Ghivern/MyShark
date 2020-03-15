#ifndef DISSECTORUDP_H
#define DISSECTORUDP_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"
#include "../../stream/tustream.h"

class DissectorUdp
{
public:
    DissectorUdp();

    static void Dissect(DissRes *dissRes, ProTree *proTree ,Info *info);

private:
    static quint32 flags;
    static TUStream stream;

    static udp_hdr* GetUdpHdr(DissRes *dissRes);
    static ushort GetUdpSrcPort(udp_hdr *header);
    static ushort GetUdpDstPort(udp_hdr *header);

};

#endif // DISSECTORUDP_H
