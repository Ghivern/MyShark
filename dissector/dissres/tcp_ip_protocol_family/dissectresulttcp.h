#ifndef DISSECTRESULTTCP_H
#define DISSECTRESULTTCP_H

#include "../dissectresultbase.h"

/*0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *Source Port                    | Destination Port              |
 * ---------------------------------------------------------------
 *Sequence Number                |  Acknowledgement Number       |
 * ---------------------------------------------------------------
 * Data  |Reserved   |U|A|P|R|S|F|   Window                      |
 * Offset|           |R|C|S|S|Y|I|                               |
 *       |           |G|K|H|T|N|N|                               |
 * ---------------------------------------------------------------
 * Checksum                      |     Urgent Point              |
 * ---------------------------------------------------------------
 * Options    .  Padding                                         |
 * ---------------------------------------------------------------
 * data ...                                                      |
 *----------------------------------------------------------------
 *
 *
 * Pseudo Header for Ipv4 Format as follow
 * --------------|---------------|---------------|----------------
 * Source Address                                                |
 * ---------------------------------------------------------------
 * Destination Address                                           |
 * ---------------------------------------------------------------
 * Zero          | Protocol      |  Up-Layer Packet Length       |
 * ---------------------------------------------------------------
 *
 *
 * Pseudo Header for Ipv4 Format as follow
 * --------------|---------------|---------------|----------------
 * Source Address                                                >
 *                                                               >
 *                                                               >
 *                                                               |
 * ---------------------------------------------------------------
 * Destination Address                                           >
 *                                                               >
 *                                                               >
 *                                                               |
 * ---------------------------------------------------------------
 * Up-Layer Packet Length                                        |
 * ---------------------------------------------------------------
 * Zero                                          | Next Header   |
 * ---------------------------------------------------------------
 */

namespace tcp_ip_protocol_family {

class DissectResultTcp
{
public:
    DissectResultTcp(DissectResultBase *dissectResultBase);
};

}
#endif // DISSECTRESULTTCP_H
