#ifndef CHECKSUM2_H
#define CHECKSUM2_H

#include <arpa/inet.h>
#include "../dissector/dissres/dissectresultbase.h"
#include "../units/converter.h"
#include "../units/ipv4info.h"
#include "../units/keys.h"
#include "../units/copy.h"

class Checksum2
{
public:
    Checksum2(DissectResultBase *dissectResultBase,QString protocol);
    ~Checksum2();

    quint16 GetCalculateChecksum();
    QString GetCalculateChecksumStr();


private:
    quint8* producePseudoHeader();
    void getInverseSum(quint16 *sum,quint16 num);
    quint16 getCalculateChecksum(const quint8 *data,qint32 dataLen,qint32 checksumPosition,quint8 *pseudoHeaderPtr,qint32 pseudoHeaderLength);

    const qint32 IPV6_ADDR_LEN = 16;
    const qint32 IPV4_ADDR_LEN = 4;
    const qint32 ZERO_CHAR_LEN = 1;
    const qint32 PROTOCOL_NUMBER_LEN = 1;
    const qint32 LENGTH_LEN = 2;

    const quint8 ZERO = 0;
    const quint8 TCP_PROTOCOL_NUM = 6;
    const quint8 UDP_PROTOCOL_NUM = 17;

    const qint32 IPV4_CHECKSUM_POSITION = 5;
    const qint32 TCP_CHECKSUM_POSITION = 8;
    const qint32 UDP_CHECKSUM_POSITION = 3;

    DissectResultBase *dissectResultBase;
    QString protocol;

};

#endif // CHECKSUM2_H
