#ifndef DISSECTRESULTUDP_H
#define DISSECTRESULTUDP_H

#include "../../units/checksum2.h"

#include "../../stream/stream.h"

#include "../dissectresultbase.h"

#include "../dissectresult.h"


/*0              |    1          |        2      |             3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * Source Port                   | Destination Port              |
 * Length                        |  Checksum                     |
 * Data Octets ...
 *
 *
 * Pseudo Header for Ipv4 Format as follow
 * --------------|---------------|---------------|----------------
 * Source Address                                                |
 * Destination Address                                           |
 * Zero          | Protocol      |  Up-Layer Packet Length       |
 *
 *
 * Pseudo Header for Ipv4 Format as follow
 * --------------|---------------|---------------|----------------
 * Source Address                                                >
 *                                                               >
 *                                                               >
 *                                                               |
 * Destination Address                                           >
 *                                                               >
 *                                                               >
 *                                                               |
 * Up-Layer Packet Length                                        |
 * Zero                                          | Next Header   |
 */

namespace tcp_ip_protocol_family {

class DissectResultUdp:public DissectResult
{
public:
    enum TRANSPORTLAYER_UDP_FIELD_LENGTH{
        TRANSPORTLAYER_UDP_FIELD_LENGTH_SOURCE_PORT = 2,
        TRANSPORTLAYER_UDP_FIELD_LENGTH_DESTINATION_PORT = 2,
        TRANSPORTLAYER_UDP_FIELD_LENGTH_LENGTH = 2,
        TRANSPORTLAYER_UDP_FIELD_LENGTH_CHECKSUM = 2,

        TRANSPORTLAYER_UDP_FIELD_LENGTH_TEMP_HEADER_LENGTH = 8,
    };

    DissectResultUdp(DissectResultBase *dissectResultBase,void *reserves = nullptr);

    quint8* GetSourcePortPtr();
    quint8* GetDestinationPortPtr();
    quint16 GetSourcePort();
    quint16 GetDestinationPort();

    quint16 GetLength();
    qint32 GetTotalLen();
    quint8* GetChecksumPtr();
    QString GetChecksumStr();
    QString GetCalculatedChecksum();


    static Stream& GetStreamRecorder();

private:
    void addNextLayer(DissectResultBase *dissectResultBase,void *reserves = nullptr);

    struct header_t{
        quint8 srcPort[2];
        quint8 dstPort[2];
        quint8 length[2];
        quint8 checksum[2];
    };

    struct header_t *header;

    QString calculatedChecksumStr;

    static Stream stream;
};

}

#endif // DISSECTRESULTUDP_H
