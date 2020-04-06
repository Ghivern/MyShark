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

    //Get方法
    static udp_hdr* GetUdpHdr(DissRes *dissRes,bool first=false);
    static ushort GetUdpSrcPort(udp_hdr *header);
    static ushort GetUdpDstPort(udp_hdr *header);
    static ushort GetUdpLength(udp_hdr *header);
    static ushort GetUdpCheckSum(udp_hdr *header);
    static QString GetUdpStrCheckSum(udp_hdr *header);


    //Get方法，参数直接为DissRes*
    static ushort GetUdpSrcPort(DissRes *dissRes);
    static ushort GetUdpDstPort(DissRes *header);

    //计算检验和
    static void GetInverseSum(quint16 *sum,quint16 num);
    static quint16 GetCalculateCheckSum(DissRes *dissRes);
    static QString GetStrCalculateCheckSum(DissRes *dissRes);

private:
    static const qint16 PSEUDO_HEADER_LEN = 12;

    static quint32 flags;
    static TUStream stream;



};

#endif // DISSECTORUDP_H
