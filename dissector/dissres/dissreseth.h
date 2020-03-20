#ifndef DISSRESETH_H
#define DISSRESETH_H
#include "dissres.h"

class DissResEth:public DissRes
{
public:
    enum  STREAM_POSITION {SRCTODST,DSTTOSRC};  //用于表示流方向

    DissResEth(qint64 no);
    void SetIpv6();
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
    QString GetStrIpSrc();
    QString GetStrIpDst();
    qint32 ipVersion();  //返回ip的版本，若无网络层，则返回0，此时只有Mac地址
    QString GetStrSrc();
    QString GetStrDst();
    //bool GetEthCRCRes(bool res);

private:
    bool isIpv6 = false;
    uchar macSrc[6] = {0};
    uchar macDst[6] = {0};
    bool ethCRCRes = false;
    uchar ipSrc[16] = {0};
    uchar ipDst[16] = {0};
    ushort srcPort = -1;
    ushort dstPort = -1;

    //统计流
    //<proName,streamIndex>
    QHash<QString,qint64> streamIndex;
    //<proName,streamDirection>
    QHash<QString,STREAM_POSITION>  streamDirection;
    //有待添加处理这两个记录器的方法

};

#endif // DISSRESETH_H
