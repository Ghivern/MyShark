#ifndef DISSECTORTCP_H
#define DISSECTORTCP_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"
#include "../../stream/streamrecorder.h"

class DissectorTcp
{
public:
    DissectorTcp();
    static void Dissect(DissRes *dissRes, ProTree *proTree, Info *info);

private:
    static quint32 flags;

    static tcp_hdr* GetTcpHdr(DissRes *dissRes);
    static ushort GetTcpSrcPort(tcp_hdr *header);
    static ushort GetTcpDstPort(tcp_hdr *header);
};

#endif // DISSECTORTCP_H
