#include "protreemaker.h"

ProTreeMaker::ProTreeMaker(qint32 datalink,DissectResultFrame *dissectRestltFrame,void *reserves){
    this->proTree = new ProTree();
    switch (datalink) {
        case DissectResultFrame::TCP_IP_PROTOCOL_FAMILY:
        new TcpIpProtocolFamily(this->proTree,dissectRestltFrame,reserves);
        break;
    }
}

ProTree* ProTreeMaker::GetProTree(){
    return this->proTree;
}
