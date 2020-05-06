#ifndef DISSECTRESULTUDP_H
#define DISSECTRESULTUDP_H

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
    DissectResultUdp(DissectResultBase *dissectResultBase,void *reserves = nullptr);
};

}

#endif // DISSECTRESULTUDP_H
