#ifndef DISSECTRESULTARP_H
#define DISSECTRESULTARP_H

#include "../dissectresultbase.h"

#include "../dissectresult.h"


/*0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 */

namespace tcp_ip_protocol_family {

class DissectResultArp:public DissectResult
{
public:
    DissectResultArp(DissectResultBase *dissectResultBase,void *reserves = nullptr);
};

}


#endif // DISSECTRESULTARP_H
