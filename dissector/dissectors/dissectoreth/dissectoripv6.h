#ifndef DISSECTORIPV6_H
#define DISSECTORIPV6_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"
#include "../../stream/streamrecorder.h"

class DissectorIpv6
{
public:
    DissectorIpv6();
    static void Dissect(DissRes *dissRes, ProTree *proTree, Info *info);

    //获取ipv6_hdr方法
    static ipv6_hdr *Ipv6GetHdr(DissRes *dissRes,bool first = false);

    //获取StramRecorder
    static StreamRecorder& Ipv6GetStreamRecorder();

    //Get方法： 获取字段值
    static quint8 Ipv6GetVersion(ipv6_hdr *header);
    static quint8 Ipv6GetVersion(DissRes *dissRes);
    static quint8 Ipv6GetTrafficClass(ipv6_hdr *header);
    static quint8 Ipv6GetTrafficClass(DissRes *dissRes);
    static quint32 Ipv6GetFlowLabel(ipv6_hdr *header);
    static quint32 Ipv6GetFlowLabel(DissRes *dissRes);
    static quint16 Ipv6GetPayloadLength(ipv6_hdr *header);
    static quint16 Ipv6GetPayloadLength(DissRes *dissRes);
    static quint8 Ipv6GetNextHeader(ipv6_hdr *header);
    static quint8 Ipv6GetNextHeader(DissRes *dissRes);
    static quint8 Ipv6GetHopLimit(ipv6_hdr *header);
    static quint8 Ipv6GetHopLimit(DissRes *dissRes);

private:
    static quint32 flags;
    static StreamRecorder streamRecorder;

};

#endif // DISSECTORIPV6_H
