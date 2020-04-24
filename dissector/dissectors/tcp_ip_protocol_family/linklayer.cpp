#include "linklayer.h"

Linklayer::Linklayer(ProTree *proTree,tcp_ip_protocol_family::DissectResultLinkLayer *dissectResultLinklayer,void *reserves)
{
    Q_UNUSED(reserves)
        /*LinkLayer处理代码*/
        Q_UNUSED(dissectResultLinklayer)
        proTree->AddItem("frame","linklayer summery"); // -
        proTree->AddItem("frame","1.1",1); // - -
        proTree->AddItem("frame","1.2",0); // - -

        proTree->Pop(1);
}
