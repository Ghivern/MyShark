#ifndef DISSRESETH_H
#define DISSRESETH_H
#include "dissres.h"

class DissResEth:public DissRes
{
public:
    DissResEth(qint64 no);
    void SetIpv6();
    void SetMacSrc(uchar *macSrc);
    void SetMacDst(uchar *macDst);
    void SetEthCRCRes(bool res);
    void SetIpSrc(uchar *ipSrc);
    void SetIpDst(uchar *ipDst);
    void SetIpv4Src(uchar *ipSrc);
    void SetIpv4Dst(uchar *ipDst);
    void SetSrcPort(ushort srcPort);
    void SetDstPort(ushort dstPort);


    QString GetStrMacSrc();
    QString GetStrMacDst();
    bool GetEthCRCRes();
    ushort GetSrcPort();
    ushort GetDstPort();
    QString GetStrIpSrc();
    QString GetStrIpDst();
    qint32 HaveIpAddr();
    QString GetStrSrc();
    QString GetStrDst();
    bool GetEthCRCRes(bool res);


private:
    typedef enum EthfieldLen{
        MAC_ADDRESS = 6,
        IP_ADDRESS = 4,
        IPV6_ADDRESS = 16
    }EthfieldLen;

    bool isIpv6 = false;
    uchar macSrc[6] = {0};
    uchar macDst[6] = {0};
    bool ethCRCRes = false;
    uchar ipSrc[16] = {0};
    uchar ipDst[16] = {0};
    ushort srcPort = 0;
    ushort dstPort = 0;

};

#endif // DISSRESETH_H
