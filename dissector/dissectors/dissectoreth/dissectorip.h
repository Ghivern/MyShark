#ifndef DISSECTORIP_H
#define DISSECTORIP_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"

class DissectorIp
{
public:
    DissectorIp();
    static void Dissect(DissRes *dissRes, ProTree *proTree ,Info *info);
private:
    static quint32 flags;
    /*
     * bit
     * 0:    0: uncheck    1: check
     */
    static ip_hdr *GetIpHdr(DissRes *dissRes,Info *info);
    static uchar GetIpVersion(ip_hdr *header);
    static uchar GetIpHdrLen(ip_hdr *header);
    static uchar GetIpService(ip_hdr *header);
    static quint16 GetIpTotalLen(ip_hdr *header);
    static quint16 GetIpIdentification(ip_hdr *header);
    static quint16 GetIpFlags(ip_hdr *header);
    static uchar GetIpFlagsReserved(ip_hdr *header);
    static uchar GetIpFlagsDF(ip_hdr *header);
    static uchar GetIpFlagsMF(ip_hdr *header);
    static quint16 GetIpOffset(ip_hdr *header);
    static uchar GetTTL(ip_hdr *header);
    static uchar GetIpPType(ip_hdr *header);
    static quint16 GetChecksum(ip_hdr *header);

    static QString MsgIpTop(DissRes *dissRes);
    static QString MsgIpVersion(ip_hdr *header);
    static QString MsgIpHdrLen(ip_hdr *header);
    static QString MsgIpTotalLen(ip_hdr *header);
    static QString MsgIdentification(ip_hdr *header);
    static QString MsgIpTTL(ip_hdr *header);
    static QString MsgIpPType(ip_hdr *header);
    static QString MsgIpSrc(DissRes *dissRes);
    static QString MsgIpDst(DissRes *dissRes);

    static void DealIpDS(ip_hdr *header,ProTree *tree,qint32 *start);
    static void DealIpFlags(ip_hdr *header,ProTree *tree,qint32 *start);
    static void DealIpChecksum(ip_hdr *header,ProTree *tree,qint32 *start);

    static void FlagSetCheck(uchar option);
    static uchar FlagGetCheck();

    static void GetInverseSum(quint16 *sum,quint16 num);
    static quint16 GetCalculateCheckSum(ip_hdr *header);
};

#endif // DISSECTORIP_H
