#include "dissectortcp.h"

quint32 DissectorTcp::flags = 0;
TUStream DissectorTcp::stream;

DissectorTcp::DissectorTcp()
{

}

void DissectorTcp::Dissect(raw_t *raw, DissRes *dissRes, ProTree *proTree, Info *info){
    tcp_hdr *header = GetTcpHdr(raw->raw,dissRes);
    if(info == NULL){
        DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissResEth->SetDstPort(GetTcpDstPort(header));
        dissResEth->SetSrcPort(GetTcpSrcPort(header));
        DissectorTcp::stream.Append(dissResEth->GetStrIpSrc()
                                    ,dissResEth->GetSrcPort()
                                    ,dissResEth->GetStrIpDst()
                                    ,dissResEth->GetDstPort());
        dissResEth->AddToProtocolStackWithSE("tcp",sizeof(tcp_hdr));
    }else{
        Q_UNUSED(proTree)
    }
}

tcp_hdr* DissectorTcp::GetTcpHdr(uchar *packet,DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    tcp_hdr *tcp = (tcp_hdr*)(packet + dissResEth->GetHeadersLen());
    dissResEth->AddHeadersLen(sizeof(tcp_hdr));



    return tcp;
}

ushort DissectorTcp::GetTcpDstPort(tcp_hdr *header){
    return ntohs(header->dport);
}

ushort DissectorTcp::GetTcpSrcPort(tcp_hdr *header){
    return ntohs(header->sport);
}

