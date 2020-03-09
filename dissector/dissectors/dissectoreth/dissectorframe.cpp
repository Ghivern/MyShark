#include "dissectorframe.h"

quint32 DissectorFrame::flags = 0;  //赋予默认配置

ProTree* DissectorFrame::Dissect(raw_t *raw, dissResList_t *dissResList,qint64 index,Info *info){
    ProTree *proTree = NULL;
    if(info == NULL){
        qDebug() << "DissectorFrame : info == NULL";
        DissResEth *dissResEth = new DissResEth(index);
        dissResEth->SetTv(raw->pkthdr->ts);
        dissResEth->SetCapLen(raw->pkthdr->caplen);
        dissResEth->SetHeadersLen(0);
        dissResEth->AddToProtocolStack("eth");
        dissResList->append(dissResEth);
    }else{
        proTree = new ProTree();
    }
    return proTree;
}
