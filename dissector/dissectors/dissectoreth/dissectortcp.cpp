#include "dissectortcp.h"

quint32 DissectorTcp::flags = 0;
StreamRecorder DissectorTcp::streamRecorder;

DissectorTcp::DissectorTcp()
{

}

void DissectorTcp::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    tcp_hdr *header = TcpGetHdr(dissRes);
    if(info == NULL){
        DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissResEth->SetDstPort(TcpGetDstPort(header));
        dissResEth->SetSrcPort(TcpGetSrcPort(header));
        dissResEth->AddToProtocolStackWithSE("tcp",sizeof(tcp_hdr));

        //添加到流记录器
        GetStreamRecorder().Add(dissResEth->GetStrIpSrc()
                                ,dissResEth->GetStrIpDst()
                                ,dissResEth->GetSrcPort()
                                ,dissResEth->GetDstPort()
                                ,dissResEth->GetNo());
    }else{
        Q_UNUSED(proTree)
    }
}

//获取流记录器
StreamRecorder& DissectorTcp::GetStreamRecorder(){
    return streamRecorder;
}

//Get方法
tcp_hdr* DissectorTcp::TcpGetHdr(DissRes *dissRes,bool first){
    dissRes->AddHeadersLen(sizeof(tcp_hdr));
    return (tcp_hdr*)BaseGetHdr(dissRes,"tcp",sizeof (tcp_hdr),first);
}

ushort DissectorTcp::TcpGetDstPort(tcp_hdr *header){
    return ntohs(header->dport);
}

ushort DissectorTcp::TcpGetSrcPort(tcp_hdr *header){
    return ntohs(header->sport);
}

