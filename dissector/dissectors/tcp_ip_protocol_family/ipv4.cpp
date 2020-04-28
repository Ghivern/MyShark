#include "ipv4.h"

Ipv4::Ipv4(ProTree *proTree,tcp_ip_protocol_family::DissectResultIpv4 *dissectResultIpv4,void *reserves)
{
    Q_UNUSED(reserves)
        /*Ipv4处理代码*/
//        Q_UNUSED(dissectResultIpv4)
//        proTree->AddItem("frame","ipv4 summery",14,23); // -
//        proTree->AddItem("frame","1.1",1); // - -
//        proTree->AddItem("frame","1.2",0); // - -
//        proTree->AddItem("frame","1.2.1",1), // - - -
//        proTree->Pop(1); // - -
//        proTree->AddItem("frame","2.1",0); // - -
//        proTree->AddItem("frame","2.2",0); // - -

//        proTree->Pop(1);
    // Summery
    qint32 start = dissectResultIpv4->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("ipv4");
    proTree->AddItem("ipv4",QString("Internet Protocol Version %1, Src: %1, Dst: %2")
                     .arg(dissectResultIpv4->GetVersion())
                     .arg(dissectResultIpv4->GetSourceAddressStr())
                     .arg(dissectResultIpv4->GetDestinationAddressStr())
                     ,start
                     ,dissectResultIpv4->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("ipv4") - 1
                     );
}
