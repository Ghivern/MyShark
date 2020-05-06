#ifndef DISSECTRESULTIPV6_H
#define DISSECTRESULTIPV6_H

#include "../dissectresultbase.h"

#include "../dissectresult.h"

#include "dissectresulttcp.h"

/*0              |    1          |        2      |             3 |
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *version| TC            |    Flow Label                         |
 * Payload Length                | Next Header   | Hop Limit     |
 *  Source Address                                               >
 *                                                               >
 *                                                               >
 *                                                               |
 * Destination Address                                           >
 *                                                               >
 *                                                               >
 *                                                               |
 *   Payload(拓展首部/数据)
 *
 *                                       .
 */

/*
 * 有待完成，难点在Authentication和ESP相关技术的理解
 */
namespace tcp_ip_protocol_family {


class DissectResultIpv6:public DissectResult
{
public:
    enum NETWORKLAYER_IPV6_FIELD_LENGTH{
        NETWORKLAYER_IPV6_FIELD_LENGTH_VERSION_TRAFFICCLASS_FLOWLABEL = 4,
        NETWORKLAYER_IPV6_FIELD_LENGTH_PAYLOAD_LENGTH = 2,
        NETWORKLAYER_IPV6_FIELD_LENGTH_NEXT_HEADER = 1,
        NETWORKLAYER_IPV6_FIELD_LENGTH_HOP_LIMIT = 1,
        NETWORKLAYER_IPV6_FIELD_LENGTH_SOURCE_ADDRESS = 16,
        NETWORKLAYER_IPV6_FIELD_LENGTH_DESTINATION_ADDRESS = 16,

        NETWORKLAYER_IPV6_FIELD_LENGTH_FIXED_HEADER_LENGTH = 40
    };

    enum NETWORKLAYER_IPV6_PROTOCOL_TYPE{
        NETWORKLAYER_IPV6_TYPE_TCP = 6,
        NETWORKLAYER_IPV6_TYPE_UCP = 17
    };

    enum NETWORKLAYER_IPV6_EXPANDED{
        NETWORKLAYER_IPV6_EXPANDED_NO_NEXT_HEADER = 59,
        NETWORKLAYER_IPV6_EXPANDED_HOP_BY_HOP = 0,
        NETWORKLAYER_IPV6_EXPANDED_FRAGMENT  = 44,
        NETWORKLAYER_IPV6_EXPANDED_DESTINATION_OPTION = 60,
        NETWORKLAYER_IPV6_EXPANDED_ROUTING = 43,
        /*下列两种先不处理*/
        NETWORKLAYER_IPV6_EXPANDED_AUTHENTICATION = 51,
        NETWORKLAYER_IPV6_EXPANDED_ENCAPSULATING_SECURITY_PAYLOAD = 50,
    };

    DissectResultIpv6(DissectResultBase *dissectResultBase,void *reserves = nullptr);
    void AddNextLayer(DissectResultBase *dissectResultBase,NETWORKLAYER_IPV6_PROTOCOL_TYPE type);

    void* GetNextLayer();
private:
    /*0              |    1          |        2      |             3 |
     *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
     *version| TC            |    Flow Label                         |
     * Payload Length                | Next Header   | Hop Limit     |
     *  Source Address                                               >
     *                                                               >
     *                                                               >
     *                                                               |
     * Destination Address                                           >
     *                                                               >
     *                                                               >
     *                                                               |
     *   Payload(拓展首部/数据)
     *
     *                                       .
     */
    struct header_t{
       quint8 version_trafficClass_flowLabel[NETWORKLAYER_IPV6_FIELD_LENGTH_VERSION_TRAFFICCLASS_FLOWLABEL];
       quint8 payloadLength[NETWORKLAYER_IPV6_FIELD_LENGTH_PAYLOAD_LENGTH];
       quint8 nextHeader[NETWORKLAYER_IPV6_FIELD_LENGTH_NEXT_HEADER];
       quint8 hopLimit[NETWORKLAYER_IPV6_FIELD_LENGTH_HOP_LIMIT];
       quint8 srcAddr[NETWORKLAYER_IPV6_FIELD_LENGTH_SOURCE_ADDRESS];
       quint8 dstAddr[NETWORKLAYER_IPV6_FIELD_LENGTH_DESTINATION_ADDRESS];
    };

    struct header_t *header;
    DissectResultBase *dissectResultBase;
    void *protocol_family_transport_layer;
};

}
#endif // DISSECTRESULTIPV6_H
