#ifndef DISSECTRESULTLINKLAYER_H
#define DISSECTRESULTLINKLAYER_H

#include <QList>
#include <arpa/inet.h>
#include "../dissectresultbase.h"
#include "dissectresultipv4.h"
#include "dissectresultipv6.h"
#include "dissectresultarp.h"

/*0                   1                   2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *                               >                               >
 *   Destination Address         |                               >
 *                               >      Source Address           |
 *    Type                       |      Data(46 ~ 1500)          .
 *                               >      FCS                      |
 */

namespace tcp_ip_protocol_family {

class DissectResultLinkLayer
{
public:
    enum LINKLAYER_FIELD_LENGTH{
        LINKLAYER_DES_ADDR = 6,
        LINKLAYER_SRC_ADDR = 6,
        LINKLAYER_TYPE = 2,
        LINKLAYER_FCS = 4,

        LINKLAYER_TEMP_TOTAL_LEN = 14
    };

    enum LINKLAYER_PROTOCOL_TYPE{
      LINKLAYER_IPV4 = 0x0008,   // 0x0800的网络字节序
      LINKLAYER_IPV6 = 0xdd86,
      LINKLAYER_ARP = 0x0608
    };

    DissectResultLinkLayer(DissectResultBase *dissectResultBase);
    void AddNextLayer(DissectResultBase *dissectResultBase, LINKLAYER_PROTOCOL_TYPE type);

    void* GetNextLayer();
    DissectResultIpv4* GetNextLayerIpv4();
    DissectResultIpv6* GetNextLayerIpv6();
    DissectResultArp* GetNextLayerArp();

private:

    struct header_t{
        quint8 dst[LINKLAYER_DES_ADDR];
        quint8 src[LINKLAYER_SRC_ADDR];
        quint8 type[LINKLAYER_TYPE];
    };

    struct header_t *header;
    DissectResultBase *dissectResultBase;
    void *protocol_family_network_layer;
};

}



#endif // DISSECTRESULTLINKLAYER_H
