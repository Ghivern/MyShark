#include "dumper.h"

Dumper::Dumper(CapHandle *capHandle,QString path){
    this->dumper = pcap_dump_open(capHandle->GetPcapHandle(),path.toLatin1());
    if( this->dumper == nullptr )
        throw pcap_geterr(capHandle->GetPcapHandle());
}

void Dumper::Dump(const pcap_pkthdr *pkthdr, const u_char *data){
    pcap_dump((u_char*)this->dumper,pkthdr,data);
}

void Dumper::Flush(){
    qint32 status = 0;
    status = pcap_dump_flush(this->dumper);
    if( status == -1 )
        throw QString("pcap_dump_flush error");
}

void Dumper::Close(){
    pcap_dump_close(this->dumper);
}
