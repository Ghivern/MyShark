#ifndef DISSECTRESULTIPV4_H
#define DISSECTRESULTIPV4_H

#include "../dissectresultbase.h"
#include "dissectresulttcp.h"
#include "dissectresultudp.h"

/*0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *Version|Hdr Len|   DS          |     total length              |
 *   identification              |flag | offset                  |
 *   ttl         | protocol      |    header checksum            |
 *                src address                                    |
 *                dst address                                    |
 * options            .  padding                                 |
 *                    data                                       |
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * 他
 * Hdr Len :单位是4字节
 * DS: Differntiated Service   RFC 2474 3168 3260
 * offset:  片偏移
 * ttl : time to live
 * 考虑到Ipv4的首部拓展很少使用，所以暂时不做处理
 */

namespace tcp_ip_protocol_family {


class DissectResultIpv4
{
public:
    enum NETWORKLAYER_IPV4_FIELD_LENGTH{
        NETWORKLAYER_IPV4_VERSION_HDRLEN = 1,
        NETWORKLAYER_IPV4__DS = 1,
        NETWORKLAYER_IPV4_TOTALLEN = 2,
        NETWORKLAYER_IPV4_IDENT = 2,
        NETWORKLAYER_IPV4_FLAG_OFFSET = 2,
        NETWORKLAYER_IPV4_TTL = 1,
        NETWORKLAYER_IPV4_TYPE = 1,
        NETWORKLAYER_IPV4_HEADERCHECKSUM = 2,
        NETWORKLAYER_IPV4_SRCADDR = 4,
        NETWORKLAYER_IPV4_DSTADDR = 4,

        NETWORKLAYER_IPV4_TEMP_TOTAL_LEN = 20
    };

    enum NETWORKLAYER_IPV4_PROTOCOL_TYPE{
        NETWORKLAYER_IPV4_TCP = 6,
        NETWORKLAYER_IPV4_UDP = 17
    };

    DissectResultIpv4(DissectResultBase *dissectResultBase);
    void AddNextLayer(DissectResultBase *dissectResultBase, NETWORKLAYER_IPV4_PROTOCOL_TYPE type);

    void* GetNextLayer();
    DissectResultTcp* GetNextLayerTcp();
    DissectResultUdp* GetNextLayerUdp();

    quint8 GetHeaderLength();

private:
    struct header_t{
        quint8 version_hdrLen;
        quint8 DS;
        quint8 totalLen[NETWORKLAYER_IPV4_TOTALLEN];
        quint8 ident[NETWORKLAYER_IPV4_IDENT];
        quint8 flag_offset[NETWORKLAYER_IPV4_FLAG_OFFSET];
        quint8 ttl[NETWORKLAYER_IPV4_TTL];
        quint8 type[NETWORKLAYER_IPV4_TYPE];
        quint8 headerchecksum[NETWORKLAYER_IPV4_HEADERCHECKSUM];
        quint8 srcaddr[NETWORKLAYER_IPV4_SRCADDR];
        quint8 dstaddr[NETWORKLAYER_IPV4_DSTADDR];
    };

    struct header_t *header;
    DissectResultBase *dissectResultBase;
    void *protocol_family_transport_layer;
};

}
#endif // DISSECTRESULTIPV4_H
