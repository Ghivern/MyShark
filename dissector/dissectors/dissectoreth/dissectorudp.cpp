#include "dissectorudp.h"

quint32 DissectorUdp::flags = 0;
TUStream DissectorUdp::stream;

DissectorUdp::DissectorUdp()
{

}


void DissectorUdp::Dissect(raw_t *raw, DissRes *dissRes, ProTree *proTree, Info *info){
    udp_hdr *header = GetUdpHdr(raw->raw,dissRes);
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

udp_hdr* DissectorUdp::GetUdpHdr(uchar *packet,DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    udp_hdr *udp = (udp_hdr*)(packet + dissResEth->GetHeadersLen());
    dissResEth->AddHeadersLen(sizeof(udp_hdr));



    return udp;
}

ushort DissectorUdp::GetUdpDstPort(udp_hdr *header){
    return ntohs(header->dport);
}

ushort DissectorUdp::GetUdpSrcPort(udp_hdr *header){
    return ntohs(header->sport);
}
