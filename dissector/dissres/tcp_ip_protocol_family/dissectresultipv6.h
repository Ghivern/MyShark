#ifndef DISSECTRESULTIPV6_H
#define DISSECTRESULTIPV6_H

#include "../dissectresultbase.h"
#include "dissectresulttcp.h"
#include "dissectresulttcp.h"

/*0              |    1          |        2      |             3 |
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *version| TC    |            Flow Label                         |
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

class DissectResultIpv6
{
public:
    DissectResultIpv6(DissectResultBase *dissectResultBase);
};

#endif // DISSECTRESULTIPV6_H
