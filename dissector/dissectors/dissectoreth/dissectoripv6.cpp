#include "dissectoripv6.h"

quint32 DissectorIpv6::flags = 0;

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
