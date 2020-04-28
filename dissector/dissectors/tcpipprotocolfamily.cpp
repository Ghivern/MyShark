#include "tcpipprotocolfamily.h"

TcpIpProtocolFamily::TcpIpProtocolFamily(ProTree *proTree, DissectResultFrame *dissectResultFrame, void *reserves)
{
    free(new Frame(proTree,dissectResultFrame,reserves));

    tcp_ip_protocol_family::DissectResultLinkLayer *linklayer = dissectResultFrame->GetTcpIpProtocolFamilyBaseLayer();
    free(new Linklayer(proTree,linklayer,reserves));

    switch (*(quint16*)linklayer->GetTypePtr()) {
    case tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_TYPE_IPV4:
    {
        tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)linklayer->GetNextLayer();
        free(new Ipv4(proTree,ipv4,reserves));
        this->dealIpv4Type(proTree,ipv4,reserves);
        break;
    }
    case tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_TYPE_ARP:
    {
        tcp_ip_protocol_family::DissectResultArp *arp = (tcp_ip_protocol_family::DissectResultArp*)linklayer->GetNextLayer();
        free(new Arp(proTree,arp,reserves));
        break;
    }
    default:
    {
        break;
    }

    }

}

void TcpIpProtocolFamily::dealIpv4Type(ProTree *proTree, tcp_ip_protocol_family::DissectResultIpv4 *ipv4, void *reserves){
    switch (ipv4->GetType()) {
    case tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_TYPE_TCP:
    {
        tcp_ip_protocol_family::DissectResultTcp *tcp = (tcp_ip_protocol_family::DissectResultTcp*)ipv4->GetNextLayer();
        free( new Tcp(proTree,tcp,reserves) );
        this->dealTcpType(proTree,tcp,reserves);
        break;
    }
    case tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_TYPE_UDP:
    {
        tcp_ip_protocol_family::DissectResultUdp *udp = (tcp_ip_protocol_family::DissectResultUdp*)ipv4->GetNextLayer();
        free(new Udp(proTree,udp,reserves));
        break;
    }
    default:
    {
        break;
    }
    }
}

void TcpIpProtocolFamily::dealTcpType(ProTree *proTree, tcp_ip_protocol_family::DissectResultTcp *tcp, void *reserves){
    if( tcp->GetNextLayer() ){
        switch (tcp->GetServPort()) {
        case tcp_ip_protocol_family::DissectResultTcp::HTTP:
        {
            DissectResultHttp *http = (DissectResultHttp*)tcp->GetNextLayer();
            free(new Http(proTree,http,reserves));
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

void TcpIpProtocolFamily::dealUdpType(ProTree *proTree, tcp_ip_protocol_family::DissectResultUdp *udp, void *reserves){
    Q_UNUSED(proTree)
    Q_UNUSED(udp)
    Q_UNUSED(reserves)
}
