#ifndef ETH_HEADER_H
#define ETH_HEADER_H
#include <QtCore>

typedef struct eth_hdr{
    uchar dst_mac[6];
    uchar src_mac[6];
    ushort eth_type;
}eth_hdr;

typedef enum ETH_LENS{
    DSTMAC = 6,
    SRCMAC = 6,
    ETHTYPE = 2,
    LGIG = 3
}ETH_LENS;

typedef struct arp_hdr{
    ushort hardware_type;
    ushort protocol_type;
    uchar hardware_address_len;
    uchar protocol_address_len;
    ushort op;
    uchar src_hardware[6];
    uchar src_protocol[4];
    uchar dst_hardware[6];
    uchar dst_protocol[4];
}arp_hdr;

typedef enum ARP_LENS{
    HARDWARE_TYPE = 2,
    PROTOCOL_TYPE = 2,
    HARDWARE_ADDRESS_LEN = 1,
    PROTOCOL_ADDRESS_LEN = 1,
    OPCODE = 2,
    SRC_HARDWARE = 6,
    SRC_PROTOCOL = 4,
    DST_HARDWARE = 6,
    DST_PROTOCOL = 4
}ARP_LENS;

typedef struct ip_hdr{
    uchar versionAndHeaderLen;  // 4:4
    uchar tos;
    ushort total_len;
    ushort ident;
    ushort flagsAndOffest;  //3:13
    uchar ttl;
    uchar protocol;
    ushort checksum;
    uchar sourceIP[4];
    uchar destIP[4];
}ip_hdr;

typedef enum IP_LENS{
    VERSION_ANDH_EADERLENGTH = 1,
    TOS = 1,
    TOTALLEN = 2,
    IDENT = 2,
    FLAGS_AND_OFFSET = 2,
    TTL = 1,
    PROTOCOL = 1,
    CHECKSUM = 2,
    SOURCEIP = 4,
    DESTIP = 4,

    IPV4_ADDRESS = 4
}IP_LENS;

typedef struct ipv6_hdr{
    quint32 vtf;  // version : 4 --> traffic class : 8 --> flow label : 20
    quint16 payloadLength;
    quint8 nextHeader;
    quint8 hopLimit;
    quint8 srcAddress[16];
    quint8 dstAddress[16];
}ipv6_hdr;

typedef enum IPV6_LEN{
    VERSION_BIT = 4,
    TRAFFIC_CLASS_BIT = 8,
    FLOW_LABEL_BIT = 20,
    PAYLOAD_LENGTH_BIT = 16,
    NEXT_HEADER_BIT = 8,
    HOP_LIMIT_BIT = 8,
    SRC_ADDRESS_BIT = 128,
    DST_ADDRESS_BIT = 128,

    //单位： byte
    VTS = 4,
    PAYLOAD_LENGTH = 2,
    NEXT_HEADER = 1,
    HOP_LIMIT = 1,
    SRC_ADDRESS = 16,
    DST_ADDRESS = 16,
    ADDRESS = 16,

    BASE_HEADER = 40
} IPV6_LEN;

typedef struct tcp_hdr
{
    ushort sport;
    ushort dport;
    uint seq;
    uint ack;
    uchar offsetAndRes;
    uchar resAndFlags;
    ushort wind_size;
    ushort check_sum;
    ushort urg_ptr;
}tcp_hdr;

typedef enum TCP_LEN{
    TCP_SRC_PORT = 2,
    TCP_DST_PORT = 2,
    TCP_SEQ = 4,
    TCP_ACK = 4,
    TCP_HEADER_LEN_AND_RES = 1,
    TCP_RES_AND_FLAGS = 1,
    TCP_WIND_SIZE = 2,
    TCP_TCP_CHECK_SUM = 2,
    TCP_URG_PTR = 2
}TCP_LEN;

typedef struct udp_hdr
{
    ushort sport;
    ushort dport;
    ushort tot_len;
    ushort check_sum;
}udp_hdr;

typedef enum UDP_LENS{
    SRC_PORT = 2,
    DST_PORT = 2,
    LENGTH = 2,
    CHECK_SUM = 2
}UDP_LENS;
#endif // PRO_HEADERS_H

