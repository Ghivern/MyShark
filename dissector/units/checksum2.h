#ifndef CHECKSUM2_H
#define CHECKSUM2_H

#include <arpa/inet.h>
#include "../dissector/dissres/dissectresultbase.h"
#include "../units/converter.h"
#include "../units/ipv4info.h"
#include "../units/keys.h"

class Checksum2
{
public:
    Checksum2(DissectResultBase *dissectResultBase,QString protocol);
    ~Checksum2();

    quint16 GetCalculateChecksum();
    QString GetCalculateChecksumStr();


private:
    void producePseudoHeader();
    void getInverseSum(quint16 *sum,quint16 num);
    quint16 getCalculateChecksum(const quint8 *data,qint32 dataLen,qint32 checksumPosition,quint8 *pseudoHeaderPtr,qint32 pseudoHeaderLength);

    const qint32 IPV6_ADDR_LEN = 16;
    const qint32 IPV4_ADDR_LEN = 4;
    const qint32 ZERO_CHAR = 1;
    const qint32 PROTOCOL_NUMBER = 1;
    const qint32 LENGTH = 2;

    const qint32 IPV4_CHECKSUM_POSITION = 5;
    const qint32 TCP_CHECKSUM_POSITION = 8;
    const qint32 UDP_CHECKSUM_POSITION = 3;

    DissectResultBase *dissectResultBase;
    QString protocol;

};

#endif // CHECKSUM2_H
