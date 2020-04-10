#ifndef DISSECTORTCP_H
#define DISSECTORTCP_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../dissectorbase.h"
#include "../../dissres/dissreseth.h"
#include "../../stream/streamrecorder.h"

class DissectorTcp :  public DissectorBase
{
public:
    DissectorTcp();
    static void Dissect(DissRes *dissRes, ProTree *proTree, Info *info);

    //获取流记录器
    static StreamRecorder& GetStreamRecorder();

    //Get方法
    static tcp_hdr* TcpGetHdr(DissRes *dissRes,bool first = false);
    static ushort TcpGetSrcPort(tcp_hdr *header);
    static ushort TcpGetDstPort(tcp_hdr *header);

private:
    static quint32 flags;
    static StreamRecorder streamRecorder;


};

#endif // DISSECTORTCP_H
