#ifndef DISSECTRESULTLINKLAYER_H
#define DISSECTRESULTLINKLAYER_H

#include <QList>
#include <QFile>
#include <QTextStream>
#include <arpa/inet.h>

#include "../dissectresultbase.h"

#include "../dissectresult.h"

#include "dissectresultipv4.h"
#include "dissectresultipv6.h"
#include "dissectresultarp.h"

/*解析器选项*/
#define ETHERNET_VALIDATE_FCS 0X0001

/*0                   1                   2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *                               >                               >
 *   Destination Address         |                               >
 *                               >      Source Address           |
 *    Type                       |      Data(46 ~ 1500)          .
 *                               >      FCS                      >
 * ---------------------------------------------------------------
 *
 * Mac Address
 *  0 | 1 | 2 | 3 | 4 | 5 |
 * ------------------------
 * byte 0 ~ 2 : OUT(Organizationally Unique Identifier)/company_id
 * byte 3 ~ 5 : extended identifier
 * byte 0 ~ 5 : EUI(Extended Unique Identifier)
 * byte0:bit0 : I/G位(Individual/Group),为0表示地址字段表示一个单个站地址(Individual address[unicast]；
 *                          为1表示组地址，用来进行多播(Group address[multicast/broadcast])
 * byte0:bit1 : G/L位(Global/Local),为0表示全球管理（Global unique address）；为1表示本地管理(Local administered address)
 *
 */

namespace tcp_ip_protocol_family {

class DissectResultLinkLayer:public DissectResult
{
public:
    enum LINKLAYER_FIELD_LENGTH{
        LINKLAYER_FIELD_LENGTH_DES_ADDR = 6,
        LINKLAYER_FIELD_LENGTH_SRC_ADDR = 6,
        LINKLAYER_FIELD_LENGTH_TYPE = 2,
        LINKLAYER_FIELD_LENGTH_FCS = 4,

        LINKLAYER_FIELD_LENGTH_TEMP_TOTAL_LEN = 14
    };

    /*凡是为了标识包字段的多字节数据，均定义为网络字节序*/
    enum LINKLAYER_PROTOCOL_TYPE{
      LINKLAYER_TYPE_IPV4 = 0x0008,
      LINKLAYER_TYPE_IPV6 = 0xdd86,
      LINKLAYER_TYPE_ARP = 0x0608
    };

    DissectResultLinkLayer(DissectResultBase *dissectResultBase,void *reserves = nullptr);

//    void* GetNextLayer();
//    DissectResultBase* GetDissectResultBase();

    /*获取协议首部字段位置或值的方法*/
    const quint8* GetSourceAddressPtr();
    const quint8* GetDestinationAddressPtr();
    QString GetSourceAddressOriginalStr();
    QString GetDestinationAddressOriginalStr();
    QString GetSourceAddressStr();
    QString GetDestinationAddressStr();

    /*处理MAC地址的I/G和G/L位*/
    bool SourceAddressIsGroup();
    bool DestinationAddressIsGroup();
    bool SourceAddressIsLocalAdministered();
    bool DestinationAddressIsLocalAdministered();
    QString GetSourceAddressLGBitStr();
    QString GetSourceAddressLGBit_meaning();
    QString GetSourceAddressLGBit_short_meaning();
    QString GetSourceAddressIGBitStr();
    QString GetSourceAddressIGBit_meaning();
    QString GetSourceAddressIGBit_short_meaning();
    QString GetDestinationAddressLGBitStr();
    QString GetDestinationAddressLGBit_meaning();
    QString GetDestinationAddressLGBit_short_meaning();
    QString GetDestinationAddressIGBitStr();
    QString GetDestinationAddressIGBit_meaning();
    QString GetDestinationAddressIGBit_short_meaning();


    const quint8* GetTypePtr();
    QString GetTypeStr();
    QString GetTypeName();

    /*处理FCS*/
    bool HaveFCS();
    const quint8* GetFCSPtr();
    QString GetFCSStr();
    /*结果是主机字节序*/
    quint32 GetCalculatedFCS();
    QString GetCalculatedFCSStr();

    /*获取流记录器*/
    static Stream& GetStreamRecorder();
    static void Clear();

private:
    struct header_t{
        quint8 dst[LINKLAYER_FIELD_LENGTH_DES_ADDR];
        quint8 src[LINKLAYER_FIELD_LENGTH_SRC_ADDR];
        quint8 type[LINKLAYER_FIELD_LENGTH_TYPE];
    };

    void addNextLayer(DissectResultBase *dissectResultBase, LINKLAYER_PROTOCOL_TYPE type,void *reserves = nullptr);

    QString getAddressOriginalStr(quint8 *address);
    QString getAddressStr(quint8 *address);

    /*
     * GroupAddress -> true -> 1; IndividualAddress ->false -> 0
     * LocalAdministeredAddress -> true -> 1; GlobalUniqueAddress -> false -> 0
     */
    bool isGroupAddress(const quint8 *address);
    bool isLocalAdministeredAddress(const quint8 *address);

    /*处理IG和LG bit*/
    QString getAddressLGBbitStr(const quint8 *address);
    QString getAddressLGbit_short_meaning(const quint8 *address);
    QString getAddressLGbit_meaning(const quint8 *address);
    QString getAddressIGBitStr(const quint8 *address);
    QString getAddressIGbit_short_meaning(const quint8 *address);
    QString getAddressIGbit_meaning(const quint8 *address);

    /*解析MAC的OUI或者熟知MAC地址*/
    static const QString relative_dir;
    static const QString ethernet_address_file_path;
    static const QString ethernet_well_know_address_file_path;

    static QFile file_ethernet_address;
    static QFile file_ethernet_well_know_address;

    static QHash<QString,QString> ethernet_address_modify_hash;

    /*Mac流记录器*/
    static Stream stream;

    bool haveFCS;

    /*解析结果的基本元素*/
    struct header_t *header;
//    DissectResultBase *dissectResultBase;

//    void *protocol_family_network_layer;
};

}



#endif // DISSECTRESULTLINKLAYER_H
