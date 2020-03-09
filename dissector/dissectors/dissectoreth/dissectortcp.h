#ifndef DISSECTORTCP_H
#define DISSECTORTCP_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../../global/global.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"
#include "../../stream/tustream.h"

class DissectorTcp
{
public:
    DissectorTcp();
    static void Dissect(raw_t *raw, DissRes *dissRes, ProTree *proTree, Info *info);

private:
    static quint32 flags;
    static TUStream stream;

    static tcp_hdr* GetTcpHdr(uchar *packet,DissRes *dissRes);
    static ushort GetTcpSrcPort(tcp_hdr *header);
    static ushort GetTcpDstPort(tcp_hdr *header);
};

#endif // DISSECTORTCP_H
