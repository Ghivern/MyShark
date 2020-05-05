#ifndef DUMPER_H
#define DUMPER_H

#include "pcap.h"

#include "caphandle.h"

class Dumper
{
public:
    Dumper();
    Dumper(CapHandle *capHandle,QString path);
    void UpdateDumper(CapHandle *capHandle,QString path);

    void Dump(const pcap_pkthdr *pkthdr,const u_char *data);
    void Flush();
    void Close();

private:
    pcap_dumper_t *dumper;
};

#endif // DUMPER_H
