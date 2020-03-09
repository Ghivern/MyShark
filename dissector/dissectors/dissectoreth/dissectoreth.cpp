#include "dissectoreth.h"
#include "dissectorarp.h"
#include "dissectorip.h"

quint32 DissectorEth::flags = 0;


DissectorEth::DissectorEth()
{

}

ProTree* DissectorEth::Dissect(rawList_t *rawList, dissResList_t *dissResList,qint64 index, Info *info){
    ProTree *proTree =  DissectorFrame::Dissect(rawList->at(index),dissResList,index,info);
    eth_hdr *header = GetHdr(rawList->at(index)->raw,dissResList->at(index));
    DissResEth *dissResEth = ((DissResEth*)dissResList->at(index));
    if(info == NULL){
        //qDebug("DissectorEth: info == NULL");
        dissResEth->SetMacDst(header->dst_mac);
        dissResEth->SetMacSrc(header->src_mac);
        dissResEth->AddToProtocolStackWithSE("ethertype",sizeof (eth_hdr));
    }else{
        proTree->AddItem("ethertype",DissectorEth::MsgTop(dissResList->at(index))
                         ,dissResEth->GetProStart("ethertype"),dissResEth->GetProEnd("ethertype"));
    }
    //qDebug() << "DissectorEth :(关注)" << ((ushort)0x0806 == GetEthPtoType(header));
    switch (GetPtoType(header)) {
    case (ushort)0x0800:
        DissectorIp::Dissect(rawList->at(index),dissResList->at(index),proTree,info);
        break;
    case (ushort)0x0806:
        DissectorArp::Dissect(rawList->at(index),dissResList->at(index),proTree,info);
        break;
    default:
        ((DissResEth*)dissResList->at(index))->SetMsg("ETH: 未知上层协议");
        break;
    }
    return proTree;
}

//Get方法
eth_hdr* DissectorEth::GetHdr(uchar *packet,DissRes *dissRes){
    eth_hdr *header = (eth_hdr*)(packet);
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    dissResEth->AddHeadersLen(sizeof(eth_hdr));

    return header;
}

ushort DissectorEth::GetPtoType(eth_hdr *header){
    return ntohs(header->eth_type);
}

//Msg方法
QString DissectorEth::MsgTop(DissRes *dissRes){
    DissResEth *dissResEth = ((DissResEth*)dissRes);
    return QString("Ethernet II : Src : ")
            + dissResEth->GetStrMacSrc() + " , "
            + "Dst : "
            + dissResEth->GetStrMacDst();
}
