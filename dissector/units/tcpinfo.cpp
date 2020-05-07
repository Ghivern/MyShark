#include "tcpinfo.h"

TcpInfo::TcpInfo()
{
    this->seq = 0;
    this->ack = 0;
    this->segLen = 0;
    this->totalLen = 0;

    this->ACK = false;
    this->SYN = false;
    this->RST = false;
    this->FIN = false;

    this->windowVal = 0;
    this->windowSclae = -1;

    this->time.tv_sec = 0;
    this->time.tv_usec = 0;
    this->echoReplayTime.tv_sec = 0;
    this->echoReplayTime.tv_usec = 0;

    this->status = 0;
    this->badChecksum = false;

    this->dupack_frame = 0;
    this->dupack_num = 0;

    this->streamPlusOne = 0;
}
