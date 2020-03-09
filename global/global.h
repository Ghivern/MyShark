#ifndef GLOBAL_H
#define GLOBAL_H
#include <QDebug>
#include "pcap.h"
#include "../dissector/dissres/dissres.h"


typedef struct raw{
    uchar *raw;
    pcap_pkthdr *pkthdr;
}raw_t;

typedef QList<raw_t*> rawList_t;

typedef QList<DissRes*> dissResList_t;
#endif // GLOBAL_H
