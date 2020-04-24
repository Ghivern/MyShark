#include "ipv4.h"

Ipv4::Ipv4(ProTree *proTree,tcp_ip_protocol_family::DissectResultIpv4 *dissectResultIpv4,void *reserves)
{
    Q_UNUSED(reserves)
        /*Ipv4处理代码*/
        Q_UNUSED(dissectResultIpv4)
        proTree->AddItem("frame","ipv4 summery"); // -
        proTree->AddItem("frame","1.1",1); // - -
        proTree->AddItem("frame","1.2",0); // - -
        proTree->AddItem("frame","1.2.1",1), // - - -
        proTree->Pop(1); // - -
        proTree->AddItem("frame","2.1",0); // - -
        proTree->AddItem("frame","2.2",0); // - -

        proTree->Pop(1);
}
