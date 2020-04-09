#ifndef DISSRESETH_H
#define DISSRESETH_H
#include "dissres.h"
#include "../dissectors/dissectoreth/eth_header.h"

class DissResEth:public DissRes
{
public:
    enum  STREAM_POSITION {SRCTODST,DSTTOSRC};  //用于表示流方向

    DissResEth(qint64 no);
    void SetIpVersion(qint32 version);
    void SetMacSrc(uchar *macSrc);
    void SetMacDst(uchar *macDst);
    void SetEthCRCRes(bool res);
    void SetIpSrc(uchar *ipSrc);
    void SetIpDst(uchar *ipDst);
    void SetIpv6Src(uchar *ipSrc);
    void SetIpv6Dst(uchar *ipDst);
    void SetSrcPort(ushort srcPort);
    void SetDstPort(ushort dstPort);

    QString GetStrMacSrc();
    QString GetStrMacDst();
    bool GetEthCRCRes();
    ushort GetSrcPort();
    ushort GetDstPort();

    //Ipv4
    quint32 GetIpSrcAddress();
    quint32 GetIpDstAddress();
    QString GetStrIpSrc();
    QString GetStrIpDst();

    //Ipv6
    QString GetStrIpv6Address(quint8 *address);
    QString GetStrIpv6Src(); // +
    QString GetStrIpv6Dst();  // +
    void GetIpv6Address(quint8 *address,quint64 *high,quint64 *low);//+
    void GetIpv6SrcAddress(quint64 *high,quint64 *low);//+
    void GetIpv6DstAddress(quint64 *hide,quint64 *low);//+
    void GetIpv6AddressNetworkOrder(quint8 *address,quint64 *high,quint64 *low);//+
    void GetIpv6SrcAddressNetworkOrder(quint64 *high,quint64 *low); // +
    void GetIpv6DstAddressNetworkOrder(quint64 *high,quint64 *low); // +

    qint32 ipVersion();  //返回ip的版本，若无网络层，则返回0，此时只有Mac地址
    QString GetStrSrc();
    QString GetStrDst();
    //bool GetEthCRCRes(bool res);

private:
    qint32 version = 0;
    uchar macSrc[6] = {0};
    uchar macDst[6] = {0};
    bool ethCRCRes = false;
    uchar ipSrc[16] = {0};
    uchar ipDst[16] = {0};
    ushort srcPort = -1;
    ushort dstPort = -1;

};

#endif // DISSRESETH_H
