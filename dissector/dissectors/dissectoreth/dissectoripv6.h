#ifndef DISSECTORIPV6_H
#define DISSECTORIPV6_H
#include <QtCore>
#include "../../protree/protree.h"
#include "../../info/infoeth.h"
#include "eth_header.h"
#include "../../dissres/dissreseth.h"

class DissectorIpv6
{
public:
    DissectorIpv6();
    static void Dissect(DissRes *dissRes, ProTree *proTree, Info *info);

private:
    static quint32 flags;

};

#endif // DISSECTORIPV6_H
