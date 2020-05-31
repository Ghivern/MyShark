#ifndef DISSECTRESULTARP_H
#define DISSECTRESULTARP_H

#include "arpa/inet.h"

#include "../dissectresultbase.h"

#include "../dissectresult.h"


/*0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * 硬件类型                        |      上层协议                  |
 * MAC地址长度    |   Ip地址长度    |     操作类型                    |
 * 源Mac
 *                                |  源头Ip
 *                                |  目的Mac
 *                  2                                               |
 * 目的Ip                                                          |
 */

namespace tcp_ip_protocol_family {

class DissectResultArp:public DissectResult
{
public:
    enum ARP_FIELD_LENGTH{
      HARDWARETYPE = 2,
      PROTOCOLTYPE = 2,
      MACADDRLEN = 1,
      PROTOCOLADDRLEN = 1,
      OPCODE = 2,
      SRCMAC = 6,
      SRCIP = 4,
      DSTMAC= 6,
      DSTIP = 4,

      ARP_TEMP_TOTAL_LEN = 28
    };

    DissectResultArp(DissectResultBase *dissectResultBase,void *reserves = nullptr);

    quint16 GetHardwareType();
    QString GetHardwareTypeStr();
    QString GetProtocolTypeName();
    QString GetProtocolTypeStr();

    quint8 GetMacAddrLen();
    quint8 GetProtocolAddrLen();

    quint16 GetOpCode();
    QString GetOpereation();

    QString GetSrcMacStr();
    QString GetSrcIpStr();
    QString GetDstMacStr();
    QString GetDstIpStr();

    QString GetSummery();
private:
    struct header_t{
      quint8 hardwareType[HARDWARETYPE];
      quint8 protocolType[PROTOCOLTYPE];
      quint8 macAddrLen[MACADDRLEN];
      quint8 protocolAddrLen[PROTOCOLADDRLEN];
      quint8 opcode[OPCODE];
      quint8 srcMac[SRCMAC];
      quint8 srcIp[SRCIP];
      quint8 dstMac[DSTMAC];
      quint8 dstIp[DSTIP];
    };

    struct header_t *header;
};

}


#endif // DISSECTRESULTARP_H
