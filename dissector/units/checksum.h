#ifndef CHECKSUM_H
#define CHECKSUM_H
#include "../dissres/dissreseth.h"
/* Ipv4 只计算首部
 * Ipv6 没有检验和字段
 *
 * 伪首部(Pseudo Header)：  源IP地址
 *                         目的IP地址
 *                         0(1 byte)|Ip首部中的protocolNumber(1byte)|Tcp/Udp数据报length
 * TCP/UDP:  伪首部 + TCP/UDP数据报
 */

class CheckSum
{
public:
    CheckSum(DissResEth *dissResEth,quint8 protocolNumber,QString proName,quint16 length); //带伪首部的检验和计算

    CheckSum(DissResEth *dissResEth,QString proName,quint16 length); //不带伪首部

    ~CheckSum();
private:
    const qint32 ZERO_CHAR = 1;
    const qint32 PROTOCOL_NUMBER = 1;
    const qint32 LENGTH = 2;

    quint8 *pseudoHeader;
    qint16 pseudoHeaderLength;

    const quint8 *data;
    quint16 length;  //进行校验和运算的数据长度
    QString proName;
};

#endif // CHECKSUM_H
