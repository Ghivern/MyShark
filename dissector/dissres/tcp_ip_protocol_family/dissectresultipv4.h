#ifndef DISSECTRESULTIPV4_H
#define DISSECTRESULTIPV4_H

#include "arpa/inet.h"
#include "time.h"

#include "../../units/checksum2.h"

#include "../dissectresultbase.h"

#include "../dissectresult.h"

#include "dissectresulttcp.h"
#include "dissectresultudp.h"

/*0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *Version|Hdr Len|   DS          |     total length              |
 *   identification              |flag | offset                  |
 *   ttl         | protocol      |    header checksum            |
 *                src address                                    |
 *                dst address                                    |
 * options            .  padding                                 |
 *                    data                                       |
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 * 他
 * Hdr Len :单位是4字节
 * DS: Diffenertiated Service   RFC 2474 3168 3260
 * offset:  片偏移
 * ttl : time to live
 * 考虑到Ipv4的首部拓展很少使用，所以暂时不做处理
 */

/*Ipv4数据包状态*/
#define IPV4_A_BAD_CHECKSUM 0X0001

/* Differentiated Services Field. See RFCs 2474, 2597, 2598 and 3168. */
/*bit0~1 ECN
 *bit2~7 DSCP
 */
#define IPDSFIELD_DSCP_DEFAULT  0x00
#define IPDSFIELD_DSCP_LE       0x01
#define IPDSFIELD_DSCP_CS1      0x08
#define IPDSFIELD_DSCP_AF11     0x0A
#define IPDSFIELD_DSCP_AF12     0x0C
#define IPDSFIELD_DSCP_AF13     0x0E
#define IPDSFIELD_DSCP_CS2      0x10
#define IPDSFIELD_DSCP_AF21     0x12
#define IPDSFIELD_DSCP_AF22     0x14
#define IPDSFIELD_DSCP_AF23     0x16
#define IPDSFIELD_DSCP_CS3      0x18
#define IPDSFIELD_DSCP_AF31     0x1A
#define IPDSFIELD_DSCP_AF32     0x1C
#define IPDSFIELD_DSCP_AF33     0x1E
#define IPDSFIELD_DSCP_CS4      0x20
#define IPDSFIELD_DSCP_AF41     0x22
#define IPDSFIELD_DSCP_AF42     0x24
#define IPDSFIELD_DSCP_AF43     0x26
#define IPDSFIELD_DSCP_CS5      0x28
#define IPDSFIELD_DSCP_EF       0x2E
#define IPDSFIELD_DSCP_CS6      0x30
#define IPDSFIELD_DSCP_CS7      0x38

#define IPDSFIELD_ECT_NOT       0x00
#define IPDSFIELD_ECT_1         0x01
#define IPDSFIELD_ECT_0         0x02
#define IPDSFIELD_CE            0x03

const QHash<qint32,QString> ecn_short_vals =
{
    { IPDSFIELD_ECT_NOT, "Not-ECT" },
    { IPDSFIELD_ECT_1,   "ECT(1)"  },
    { IPDSFIELD_ECT_0,   "ECT(0)"  },
    { IPDSFIELD_CE,      "CE"      },
};

const QHash<qint32,QString> ecn_vals =
{
    { IPDSFIELD_ECT_NOT, "Not ECN-Capable Transport"            },
    { IPDSFIELD_ECT_1,   "ECN-Capable Transport codepoint '01'" },
    { IPDSFIELD_ECT_0,   "ECN-Capable Transport codepoint '10'" },
    { IPDSFIELD_CE,      "Congestion Experienced"               }
};

const QHash<qint32,QString> dscp_short_vals =
{
    { IPDSFIELD_DSCP_DEFAULT, "CS0"    },
    { IPDSFIELD_DSCP_LE,      "LE"     },
    { IPDSFIELD_DSCP_CS1,     "CS1"    },
    { IPDSFIELD_DSCP_AF11,    "AF11"   },
    { IPDSFIELD_DSCP_AF12,    "AF12"   },
    { IPDSFIELD_DSCP_AF13,    "AF13"   },
    { IPDSFIELD_DSCP_CS2,     "CS2"    },
    { IPDSFIELD_DSCP_AF21,    "AF21"   },
    { IPDSFIELD_DSCP_AF22,    "AF22"   },
    { IPDSFIELD_DSCP_AF23,    "AF23"   },
    { IPDSFIELD_DSCP_CS3,     "CS3"    },
    { IPDSFIELD_DSCP_AF31,    "AF31"   },
    { IPDSFIELD_DSCP_AF32,    "AF32"   },
    { IPDSFIELD_DSCP_AF33,    "AF33"   },
    { IPDSFIELD_DSCP_CS4,     "CS4"    },
    { IPDSFIELD_DSCP_AF41,    "AF41"   },
    { IPDSFIELD_DSCP_AF42,    "AF42"   },
    { IPDSFIELD_DSCP_AF43,    "AF43"   },
    { IPDSFIELD_DSCP_CS5,     "CS5"    },
    { IPDSFIELD_DSCP_EF,      "EF PHB" },
    { IPDSFIELD_DSCP_CS6,     "CS6"    },
    { IPDSFIELD_DSCP_CS7,     "CS7"    },
};

const QHash<qint32,QString> dscp_vals =
{
    { IPDSFIELD_DSCP_DEFAULT, "Default"               },
    { IPDSFIELD_DSCP_LE,      "Lower Effort"          },
    { IPDSFIELD_DSCP_CS1,     "Class Selector 1"      },
    { IPDSFIELD_DSCP_AF11,    "Assured Forwarding 11" },
    { IPDSFIELD_DSCP_AF12,    "Assured Forwarding 12" },
    { IPDSFIELD_DSCP_AF13,    "Assured Forwarding 13" },
    { IPDSFIELD_DSCP_CS2,     "Class Selector 2"      },
    { IPDSFIELD_DSCP_AF21,    "Assured Forwarding 21" },
    { IPDSFIELD_DSCP_AF22,    "Assured Forwarding 22" },
    { IPDSFIELD_DSCP_AF23,    "Assured Forwarding 23" },
    { IPDSFIELD_DSCP_CS3,     "Class Selector 3"      },
    { IPDSFIELD_DSCP_AF31,    "Assured Forwarding 31" },
    { IPDSFIELD_DSCP_AF32,    "Assured Forwarding 32" },
    { IPDSFIELD_DSCP_AF33,    "Assured Forwarding 33" },
    { IPDSFIELD_DSCP_CS4,     "Class Selector 4"      },
    { IPDSFIELD_DSCP_AF41,    "Assured Forwarding 41" },
    { IPDSFIELD_DSCP_AF42,    "Assured Forwarding 42" },
    { IPDSFIELD_DSCP_AF43,    "Assured Forwarding 43" },
    { IPDSFIELD_DSCP_CS5,     "Class Selector 5"      },
    { IPDSFIELD_DSCP_EF,      "Expedited Forwarding"  },
    { IPDSFIELD_DSCP_CS6,     "Class Selector 6"      },
    { IPDSFIELD_DSCP_CS7,     "Class Selector 7"      },
};



namespace tcp_ip_protocol_family {


class DissectResultIpv4:public DissectResult
{
public:
    enum NETWORKLAYER_IPV4_FIELD_LENGTH{
        NETWORKLAYER_IPV4_FIELD_LENGTH_VERSION_HDRLEN = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_DS = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_TOTALLEN = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_IDENT = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_TTL = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_TYPE = 1,
        NETWORKLAYER_IPV4_FIELD_LENGTH_HEADERCHECKSUM = 2,
        NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR = 4,
        NETWORKLAYER_IPV4_FIELD_LENGTH_DSTADDR = 4,

        NETWORKLAYER_IPV4_FIELD_LENGTH_TEMP_TOTAL_LEN = 20
    };

    enum NETWORKLAYER_IPV4_PROTOCOL_TYPE{
        NETWORKLAYER_IPV4_TYPE_TCP = 6,
        NETWORKLAYER_IPV4_TYPE_UDP = 17
    };

    DissectResultIpv4(DissectResultBase *dissectResultBase,void *reserves = nullptr);


    /*获得协议首部位置或值的方法*/
    const quint8* GetVersionHeaderLengthPtr();
    quint8 GetVersion();
    QString GetVersionDotStr();
    /*以四字节为单位*/
    quint8 GetHeaderLength();
    QString GetHeaderLengthDotStr();

    const quint8* GetDifferentiatedServicePrt();
    QString GetDifferentiatedServiceStr();

    /*bit0~1*/
    quint8 GetDSField_ECN();
    QString GetDSField_ECN_DotStr();
    QString GetDSFField_ECN_short_meaning();
    QString GetDSField_ECN_meanning();
    /*bit2~7*/
    quint8 GetDSField_DSCP();
    QString GetDSField_DSCP_DotStr();
    QString GetDSField_DSCP_short_meaning();
    QString GetDSField_DSCP_meanning();

    const quint8* GetTotalLengthPrt();
    quint16 GetTotalLength();

    const quint8* GetIdentificationPtr();
    quint16 GetIdentification();
    QString GetIdentificationStr();

    /*flag和offset*/
    const quint8* GetFlagOffsetPtr();
    QString GetFlagOffsetStr();
    bool Reserve();
    QString GetReserveDotStr();
    QString Reserve_meaning();
    /*Don't fragment*/
    bool DF();
    QString GetDFDotStr();
    QString DF_meaning();
    /*More fragment*/
    bool MF();
    QString GetMFDotStr();
    QString MF_meaning();
    quint16 GetFragmentOffset();
    QString GetFragmentOffsetDotStr();

    const quint8* GetTimeToLivePtr();
    quint8 GetTimeToLive();

    const quint8* GetTypePtr();
    quint8 GetType();
    QString GetTypeName();

    const quint8* GetChecksumPtr();
    QString GetChecksumStr();
    QString GetCalculatedChecksumStr();

    const quint8* GetSourceAddressPtr();
    QString GetSourceAddressStr();

    const quint8* GetDestinationAddressPtr();
    QString GetDestinationAddressStr();

    static Stream& GetStreamRecorder();

private:
    struct header_t{
        quint8 version_hdrLen[NETWORKLAYER_IPV4_FIELD_LENGTH_VERSION_HDRLEN];
        quint8 DS[NETWORKLAYER_IPV4_FIELD_LENGTH_DS];
        quint8 totalLen[NETWORKLAYER_IPV4_FIELD_LENGTH_TOTALLEN];
        quint8 ident[NETWORKLAYER_IPV4_FIELD_LENGTH_IDENT];
        quint8 flag_offset[NETWORKLAYER_IPV4_FIELD_LENGTH_FLAG_OFFSET];
        quint8 ttl[NETWORKLAYER_IPV4_FIELD_LENGTH_TTL];
        quint8 type[NETWORKLAYER_IPV4_FIELD_LENGTH_TYPE];
        quint8 headerchecksum[NETWORKLAYER_IPV4_FIELD_LENGTH_HEADERCHECKSUM];
        quint8 srcaddr[NETWORKLAYER_IPV4_FIELD_LENGTH_SRCADDR];
        quint8 dstaddr[NETWORKLAYER_IPV4_FIELD_LENGTH_DSTADDR];
    };

    void addNextLayer(DissectResultBase *dissectResultBase
                      , NETWORKLAYER_IPV4_PROTOCOL_TYPE type,void *reserves = nullptr);
    //quint8* producePseudoHeader(NETWORKLAYER_IPV4_PROTOCOL_TYPE type);

    QString calculatedChecksum;

    //const qint32 pseudoHeaderLen = 12;
    static Stream stream;

    struct header_t *header;
};

}
#endif // DISSECTRESULTIPV4_H
