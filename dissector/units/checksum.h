#ifndef CHECKSUM_H
#define CHECKSUM_H
#include "../dissres/dissreseth.h"
/*
 * Pseudo Header:
 * Src Addr | Dst Addr | Zero-Char | protocol-number | length
 *
 * 协议     |   检验字段
 * -----------------------------------
 * Ipv4    |   首部
 * Ipv6    |   无
 * Tcp/Udp |   Pseudo Header + 数据报
 */

class CheckSum
{
public:
    /*
     * 生成Pseudo Header的构造函数
     * 1.目前支持TCP、UDP、IPV4
     * 2.withPseudoHeader指示是否添加伪首部，默认不添加
     * 3.length表明待检验数据的长度
     * 4.协议名称用小写 tcp、udp、ipv4
     */
    CheckSum(DissResEth *dissResEth,QString proName,quint16 length, bool withPseudoHeader = false);
    ~CheckSum();

     quint16 GetChecksum();
     QString GetStrChecksum();
private:
    quint8* GetPseudoHeader(DissResEth *dissResEth,quint8 proNumber,quint16 length);
    void GetInverseSum(quint16 *sum,quint16 num);

    const qint32 IPV6_ADDR_LEN = 16;
    const qint32 IPV4_ADDR_LEN = 4;
    const qint32 ZERO_CHAR = 1;
    const qint32 PROTOCOL_NUMBER = 1;
    const qint32 LENGTH = 2;

    const qint32 IPV4_CHECKSUM_POSITION = 5;
    const qint32 TCP_CHECKSUM_POSITION = 8;
    const qint32 UDP_CHECKSUM_POSITION = 3;

    quint8 *pseudoHeaderPtr;
    qint16 pseudoHeaderLength;

    const quint8 *data;
    quint16 dataLength;

    qint32 checksum_position;

    bool done;
    quint16 checksum;
};

#endif // CHECKSUM_H
