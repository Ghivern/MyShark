#include "dissectoripv6.h"

quint32 DissectorIpv6::flags = 0;
StreamRecorder DissectorIpv6::streamRecorder;

DissectorIpv6::DissectorIpv6()
{

}

void DissectorIpv6::Dissect(DissRes *dissRes, ProTree *proTree, Info *info){
    if(info == NULL){
        Q_UNUSED(dissRes)
    }else{
        Q_UNUSED(proTree)
    }
}

//Get方法
ipv6_hdr* DissectorIpv6::Ipv6GetHdr(DissRes *dissRes, bool first){
    if(first)
        dissRes->AddToProtocolStackWithSE("ipv6",sizeof(ipv6_hdr));
    ipv6_hdr *ipv6 = (ipv6_hdr*)(dissRes->GetData() + dissRes->GetProStart("ip"));
    return ipv6;
}

StreamRecorder& DissectorIpv6::Ipv6GetStreamRecorder(){
    return streamRecorder;
}

quint8 DissectorIpv6::Ipv6GetVersion(ipv6_hdr *header){
    quint32 vtf = ntohl(header->vtf);
    return vtf >> (IPV6_LEN::TRAFFIC_CLASS_BIT + IPV6_LEN::FLOW_LABEL_BIT);
}

quint8 DissectorIpv6::Ipv6GetVersion(DissRes *dissRes){
    return Ipv6GetVersion(Ipv6GetHdr(dissRes));
}

quint8 DissectorIpv6::Ipv6GetTrafficClass(ipv6_hdr *header){
    quint32 vtf = ntohl(header->vtf);
    return vtf >> IPV6_LEN::FLOW_LABEL_BIT;

}

quint8 DissectorIpv6::Ipv6GetTrafficClass(DissRes *dissRes){
    return Ipv6GetTrafficClass(Ipv6GetHdr(dissRes));
}

quint32 DissectorIpv6::Ipv6GetFlowLabel(ipv6_hdr *header){
    quint32 vtf = ntohl(header->vtf);
    return vtf & 0x000fffff;
}

quint32 DissectorIpv6::Ipv6GetFlowLabel(DissRes *dissRes){
    return Ipv6GetFlowLabel(Ipv6GetHdr(dissRes));
}

quint16 DissectorIpv6::Ipv6GetPayloadLength(ipv6_hdr *header){
    return ntohs(header->payloadLength);
}

quint16 DissectorIpv6::Ipv6GetPayloadLength(DissRes *dissRes){
    return Ipv6GetPayloadLength(Ipv6GetHdr(dissRes));
}

quint8 DissectorIpv6::Ipv6GetNextHeader(ipv6_hdr *header){
    return header->nextHeader;
}

quint8 DissectorIpv6::Ipv6GetNextHeader(DissRes *dissRes){
    return Ipv6GetNextHeader(Ipv6GetHdr(dissRes));
}

quint8 DissectorIpv6::Ipv6GetHopLimit(ipv6_hdr *header){
    return header->hopLimit;
}

quint8 DissectorIpv6::Ipv6GetHopLimit(DissRes *dissRes){
    return Ipv6GetHopLimit(Ipv6GetHdr(dissRes));
}
