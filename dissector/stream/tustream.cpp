#include "tustream.h"

TUStream::TUStream()
{

}

qint64 TUStream::Exist(QString srcAddress,ushort srcPort,QString dstAddress,ushort dstPort,bool& AB){
    for(qint64 index = 0; index < this->connects.length(); index++){
        bool resA = connects.at(index).addressA == srcAddress && connects.at(index).portA == srcPort
                && connects.at(index).addressB == dstAddress && connects.at(index).portB == dstPort;
        bool resB = connects.at(index).addressA == dstAddress && connects.at(index).portA == dstPort
                && connects.at(index).addressB == srcAddress && connects.at(index).portB == srcPort;
        if(resA || resB){
            if(resA)
                AB = true;
            else
                AB = false;
            return index;
        }
    }
    return -1;
}
void TUStream::Append(QString srcAddress,ushort srcPort,QString dstAddress,ushort dstPort){
    bool AB;
    qint64 index = this->Exist(srcAddress,srcPort,dstAddress,dstPort,AB);
    if( index == -1 ){
        connect_t connect;
        connect.addressA = srcAddress;
        connect.portA = srcPort;
        connect.addressB = dstAddress;
        connect.portB = dstPort;
        this->connects.append(connect);
    }
}
QString TUStream::GetAddressA(qint64 index){
    return connects.at(index).addressA;
}
QString TUStream::GetAddressB(qint64 index){
    return connects.at(index).addressB;
}
ushort TUStream::GetPortA(qint64 index){
    return connects.at(index).portA;
}
ushort TUStream::GetPortB(qint64 index){
    return connects.at(index).portB;
}

qint64 TUStream::StreamIndex(QString addressA,ushort portA,QString addressB,ushort portB){
    for(qint64 index = 0; index < this->connects.length(); index++){
        bool resA = connects.at(index).addressA == addressA && connects.at(index).portA == portA
                && connects.at(index).addressB == addressB && connects.at(index).portB == portB;
        bool resB = connects.at(index).addressA == addressB && connects.at(index).portA == portB
                && connects.at(index).addressB == addressA && connects.at(index).portB == portA;
        if(resA || resB)
            return index;
    }
    return -1;
}

