#include "tcpipprotocolfamily.h"

TcpIpProtocolFamily::TcpIpProtocolFamily(ProTree *proTree, DissectResultFrame *dissectResultFrame, void *reserves)
{
    free(new Frame(proTree,dissectResultFrame,reserves));
    free(new Linklayer(proTree,dissectResultFrame->GetTcpIpProtocolFamilyBaseLayer(),reserves));

    tcp_ip_protocol_family::DissectResultLinkLayer *linklayer = dissectResultFrame->GetTcpIpProtocolFamilyBaseLayer();
    quint16 linklayerType = *(quint16*)dissectResultFrame->GetTcpIpProtocolFamilyBaseLayer()->GetTypePtr();
    if(linklayerType == tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_TYPE_IPV4){
        tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)linklayer->GetNextLayer();
        free(new Ipv4(proTree,ipv4,reserves));
    }else{
        ;
    }
}
