#include "dissectorip.h"
#include "dissectorudp.h"
#include "dissectortcp.h"

quint32 DissectorIp::flags = 0;

DissectorIp::DissectorIp()
{

}

void DissectorIp::Dissect(raw_t *raw, DissRes *dissRes, ProTree *proTree, Info *info){
    ip_hdr *header = GetIpHdr(raw->raw,dissRes);
    if(info == NULL){
        DissResEth *dissResEth = ((DissResEth*)dissRes);
        dissResEth->SetIpSrc(header->sourceIP);
        dissResEth->SetIpDst(header->destIP);
        dissResEth->AddToProtocolStackWithSE("ip",DissectorIp::GetIpHdrLen(header));

    }else{
        Q_UNUSED(proTree)
    }

    switch (GetIpPType(header)) {
    case 6:
        DissectorTcp::Dissect(raw,dissRes,proTree,info);
        break;
    case 17:
        DissectorUdp::Dissect(raw,dissRes,proTree,info);
        break;
    default:
        break;
    }
}


//Get 方法
ip_hdr* DissectorIp::GetIpHdr(uchar *packet,DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    ip_hdr* ip = (ip_hdr*)(packet + dissResEth->GetHeadersLen());
    dissResEth->AddHeadersLen(GetIpHdrLen(ip));


    return ip;
}

uchar DissectorIp::GetIpHdrLen(ip_hdr *header){
    return (uchar)(header->versionAndHeaderLen & 0x0f) * 4;
}

uchar DissectorIp::GetIpPType(ip_hdr *header){
    return header->protocol;
}


//Flag 方法
void DissectorIp::FlagSetCheck(uchar option){
    if(option == 0)
        DissectorIp::flags &= 0xfffffffe;
    else
        DissectorIp::flags |= 0x00000001;
}

uchar DissectorIp::FlagGetCheck(){
    return (DissectorIp::flags & 0x00000001) >> 0;
}