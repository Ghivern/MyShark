#ifndef DISSECTRESULTIPV4_H
#define DISSECTRESULTIPV4_H

#include "arpa/inet.h"
#include "time.h"

#include "../converter.h"

#include "../dissectresultcommonstream.h"
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
 * DS: Diffenertiated Service   RFC 2474 3168 3260
 * offset:  片偏移
 * ttl : time to live
 * 考虑到Ipv4的首部拓展很少使用，所以暂时不做处理
 */

namespace tcp_ip_protocol_family {


class DissectResultIpv4:public DissectResultCommonStream
{
public:
    enum NETWORKLAYER_IPV4_FIELD_LENGTH{
        NETWORKLAYER_IPV4_FIELD_LENGTH_VERSION_HDRLEN = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_DS = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_TOTALLEN = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_IDENT = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_TTL = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_TYPE = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_HEADERCHECKSUM = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR = 4,
        NETWORKLAYER_IPV4_FIELD_LENGTH_DSTADDR = 4,

        NETWORKLAYER_IPV4_FIELD_LENGTH_TEMP_TOTAL_LEN = 20
    };

    enum NETWORKLAYER_IPV4_PROTOCOL_TYPE{
        NETWORKLAYER_IPV4_TYPE_TCP = 6,
        NETWORKLAYER_IPV4_TYPE_UDP = 17
    };

    DissectResultIpv4(DissectResultBase *dissectResultBase);
    void AddNextLayer(DissectResultBase *dissectResultBase, NETWORKLAYER_IPV4_PROTOCOL_TYPE type);

    void* GetNextLayer();


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
     */

    /*获得协议首部位置或值的方法*/
    const quint8* GetVersionHeaderLengthPtr();
    quint8 GetVersion();
    quint8 GetHeaderLength();

    const quint8* GetDifferentiatedServicePrt();
    QString GetDifferentiatedServiceStr();

    const quint8* GetTotalLengthPrt();
    quint16 GetTotalLength();

    const quint8* GetIdentificationPtr();
    quint16 GetIdentification();
    QString GetIdentificationStr();

    const quint8* GetFlagOffsetPtr();
    QString GetFlagOffsetStr();

    const quint8* GetTimeToLivePtr();
    quint8 GetTimeToLive();

    const quint8* GetTypePtr();
    quint8 GetType();
    QString GetTypeName();

    const quint8* GetChecksumPtr();
    QString GetChecksumStr();

    const quint8* GetSourceAddressPtr();
    QString GetSourceAddressStr();

    const quint8* GetDestinationAddressPtr();
    QString GetDestinationAddressStr();

private:
    struct header_t{
        quint8 version_hdrLen[NETWORKLAYER_IPV4_FIELD_LENGTH_VERSION_HDRLEN];
        quint8 DS[NETWORKLAYER_IPV4_FIELD_LENGTH_DS];
        quint8 totalLen[NETWORKLAYER_IPV4_FIELD_LENGTH_TOTALLEN];
        quint8 ident[NETWORKLAYER_IPV4_FIELD_LENGTH_IDENT];
        quint8 flag_offset[NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET];
        quint8 ttl[NETWORKLAYER_IPV4_FIELD_LENGTH_TTL];
        quint8 type[NETWORKLAYER_IPV4_FIELD_LENGTH_TYPE];
        quint8 headerchecksum[NETWORKLAYER_IPV4_FIELD_LENGTH_HEADERCHECKSUM];
        quint8 srcaddr[NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR];
        quint8 dstaddr[NETWORKLAYER_IPV4_FIELD_LENGTH_DSTADDR];
    };

    static Stream stream;

    struct header_t *header;
    DissectResultBase *dissectResultBase;
    void *protocol_family_transport_layer;
};

}
#endif // DISSECTRESULTIPV4_H
