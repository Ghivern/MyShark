#include "dissectorudp.h"

quint32 DissectorUdp::flags = 0;
TUStream DissectorUdp::stream;

DissectorUdp::DissectorUdp()
{

}


void DissectorUdp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    udp_hdr *header = GetUdpHdr(dissRes);
    if(info == NULL){
        DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissResEth->SetSrcPort(GetUdpSrcPort(header));
        dissResEth->SetDstPort(GetUdpDstPort(header));
        dissResEth->AddToProtocolStackWithSE("udp",sizeof(udp_hdr));
        DissectorUdp::stream.Append(dissResEth->GetStrIpSrc()
                                    ,dissResEth->GetSrcPort()
                                    ,dissResEth->GetStrIpDst()
                                    ,dissResEth->GetDstPort());
        qDebug() << "DissectorUdp : (关注 StreamIndex)" <<
                    DissectorUdp::stream.StreamIndex(dissResEth->GetStrIpSrc()
                                                     ,dissResEth->GetSrcPort()
                                                     ,dissResEth->GetStrIpDst()
                                                     ,dissResEth->GetDstPort());
    }else{
        Q_UNUSED(proTree)
    }
}

udp_hdr* DissectorUdp::GetUdpHdr(DissRes *dissRes){
    udp_hdr *udp = (udp_hdr*)(dissRes->GetData() + dissRes->GetHeadersLen());
    dissRes->AddHeadersLen(sizeof(udp_hdr));
    return udp;
}

ushort DissectorUdp::GetUdpDstPort(udp_hdr *header){
    return ntohs(header->dport);
}

ushort DissectorUdp::GetUdpSrcPort(udp_hdr *header){
    return ntohs(header->sport);
}
