#include "checksum.h"
#include "../dissectors/dissectoreth/eth_header.h"

CheckSum::CheckSum(DissResEth *dissResEth,quint8 protocolNumber,QString proName,quint16 length){
    quint8 ipVersion = dissResEth->ipVersion();
    quint8 *temp;
    quint32 addr,num;
    quint64 addrs[4];
    this->proName.append(proName);
    this->data = dissResEth->GetData() + dissResEth->GetProStart(proName);
    this->length = length;
    if(ipVersion == 4 || ipVersion == 6){
        if(ipVersion == 4){
            this->pseudoHeaderLength = IP_LENS::IPV4_ADDRESS * 2 + ZERO_CHAR + PROTOCOL_NUMBER + LENGTH;
            this->pseudoHeader = (quint8*)malloc(this->pseudoHeaderLength);

            addr = dissResEth->GetIpSrcAddressNetworkOrder();
            memcpy(this->pseudoHeader,&addr,IP_LENS::IPV4_ADDRESS);
            temp = this->pseudoHeader + IP_LENS::IPV4_ADDRESS;

            addr = dissResEth->GetIpDstAddressNetworkOrder();
            memcpy(temp,&addr,IP_LENS::IPV4_ADDRESS);
            temp += IP_LENS::IPV4_ADDRESS;
        }else{
            this->pseudoHeaderLength = IPV6_LEN::ADDRESS *  2 + ZERO_CHAR + PROTOCOL_NUMBER + LENGTH;
            this->pseudoHeader = (quint8*)malloc(this->pseudoHeaderLength);
            dissResEth->GetIpv6SrcAddressNetworkOrder(addrs + 1,addrs);
            dissResEth->GetIpv6DstAddressNetworkOrder(addrs + 3,addrs+2);
            memcpy(this->pseudoHeader,addrs,IPV6_LEN::ADDRESS*2);
            temp = this->pseudoHeader + IPV6_LEN::ADDRESS*2;
        }
        num = 0;
        memcpy(temp,&num,ZERO_CHAR);
        temp += ZERO_CHAR;

        num = protocolNumber;
        memcpy(temp,&num,PROTOCOL_NUMBER);
        temp += PROTOCOL_NUMBER;

        num = htons(length);
        memcpy(temp,&num,LENGTH);
        temp += LENGTH;
    }else{
        this->pseudoHeader = NULL;
        this->pseudoHeaderLength = 0;
    }
}

CheckSum::CheckSum(DissResEth* dissResEth,QString proName,quint16 length){
    this->length = length;
    this->proName.append(proName);
    this->pseudoHeader = NULL;
    this->pseudoHeaderLength = 0;
    this->data = dissResEth->GetData() + dissResEth->GetProStart(proName);
}
