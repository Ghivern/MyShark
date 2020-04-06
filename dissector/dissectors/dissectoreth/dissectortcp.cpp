#include "dissectortcp.h"

quint32 DissectorTcp::flags = 0;

DissectorTcp::DissectorTcp()
{

}

void DissectorTcp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    tcp_hdr *header = GetTcpHdr(dissRes);
    if(info == NULL){
        DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissResEth->SetDstPort(GetTcpDstPort(header));
        dissResEth->SetSrcPort(GetTcpSrcPort(header));
//        DissectorTcp::stream.Append(dissResEth->GetStrIpSrc()
//                                    ,dissResEth->GetSrcPort()
//                                    ,dissResEth->GetStrIpDst()
//                                    ,dissResEth->GetDstPort());
        dissResEth->AddToProtocolStackWithSE("tcp",sizeof(tcp_hdr));
    }else{
        Q_UNUSED(proTree)
    }
}

tcp_hdr* DissectorTcp::GetTcpHdr(DissRes *dissRes){
    tcp_hdr *tcp = (tcp_hdr*)(dissRes->GetData() + dissRes->GetHeadersLen());
    dissRes->AddHeadersLen(sizeof(tcp_hdr));
    return tcp;
}

ushort DissectorTcp::GetTcpDstPort(tcp_hdr *header){
    return ntohs(header->dport);
}

ushort DissectorTcp::GetTcpSrcPort(tcp_hdr *header){
    return ntohs(header->sport);
}

