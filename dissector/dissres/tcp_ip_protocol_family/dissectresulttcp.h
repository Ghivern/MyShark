#ifndef DISSECTRESULTTCP_H
#define DISSECTRESULTTCP_H

#include <QtMath>
#include "arpa/inet.h"

#include "../../units/checksum2.h"

#include "../../stream/streamtcp2.h"

#include "../dissectresultbase.h"

#include "../dissectresult.h"

#include "./dissectresulthttp.h"

/*0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *Source Port                    | Destination Port              |
 * ---------------------------------------------------------------
 *Sequence Number                                                |
 * ---------------------------------------------------------------
 *Acknowledgement Number                                         |
 * ---------------------------------------------------------------
 * Data  |Reserved   |U|A|P|R|S|F|   Window                      |
 * Offset|           |R|C|S|S|Y|I|                               |
 *       |           |G|K|H|T|N|N|                               |
 * ---------------------------------------------------------------
 * Checksum                      |     Urgent Point              |
 * ---------------------------------------------------------------
 * Options    .  Padding                                         |
 * ---------------------------------------------------------------
 * data ...                                                      |
 *----------------------------------------------------------------
 *
 */


 /* Pseudo Header for Ipv4 Format as follow
 * --------------|---------------|---------------|----------------
 * Source Address                                                |
 * ---------------------------------------------------------------
 * Destination Address                                           |
 * ---------------------------------------------------------------
 * Zero          | Protocol      |  Up-Layer Packet Length       |
 * ---------------------------------------------------------------
 *
 *
 * Pseudo Header for Ipv6 Format as follow
 * --------------|---------------|---------------|----------------
 * Source Address                                                >
 *                                                               >
 *                                                               >
 *                                                               |
 * ---------------------------------------------------------------
 * Destination Address                                           >
 *                                                               >
 *                                                               >
 *                                                               |
 * ---------------------------------------------------------------
 * Up-Layer Packet Length                                        |
 * ---------------------------------------------------------------
 * Zero                                          | Next Header   |
 * ---------------------------------------------------------------
 */




/*0                   1          |        2                   3
 *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
 *Source Port                    | Destination Port              |
 * ---------------------------------------------------------------
 *Sequence Number                                                |
 * ---------------------------------------------------------------
 *Acknowledgement Number                                         |
 * ---------------------------------------------------------------
 * Data  |Reserved   |U|A|P|R|S|F|   Window                      |
 * Offset|           |R|C|S|S|Y|I|                               |
 *       |           |G|K|H|T|N|N|                               |
 * ---------------------------------------------------------------
 * Checksum                      |     Urgent Point              |
 * ---------------------------------------------------------------
 * Options    .  Padding                                         |
 * ---------------------------------------------------------------
 * data ...                                                      |
 *----------------------------------------------------------------
 *
 */
#define TCP_OPTION_END_OF_OPTION 0x00
#define TCP_OPTION_NO_OPERATION 0x01
#define TCP_OPTION_MAXIMUM_SEGMENT_SIZE 0x02
#define TCP_OPTION_WINDOW_SCALE_OPTION 0x03
#define TCP_OPTION_SACK_PERMITTED_OPTION 0x04
#define TCP_OPTION_SACK_OPTION_FORMAT 0x05
#define TCP_OPTION_TIMESTAMPS_OPTION 0x08


namespace tcp_ip_protocol_family {

class DissectResultTcp:public DissectResult
{
public:
    static QHash<qint32,QString> tcp_option_val;

    enum TRANSPORTLAYER_TCP_FIELD_LENGTH{
        TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT = 2,
        TRANSPORTLAYER_TCP_FIELD_LENGTH_DESTINATION_PORT = 2,
        TRANSPORTLAYER_TCP_FIELD_LENGTH_SEQ = 4,
        TRANSPORTLAYER_TCP_FIELD_LENGTH_ACK = 4,
        TRANSPORTLAYER_TCP_FIELD_LENGTH_DATAOFFSET_RESERVED_FLAGS = 2,
        TRANSPORTLAYER_TCP_FIELD_LENGTH_WINDOW = 2,
        TRANSPORTLAYER_TCP_FIELD_LENGTH_CHECKSUM = 2,
        TRANSPORTLAYER_TCP_FIELD_LENGTH_URGENTPOINT = 2,

        TRANSPORTLAYER_TCP_FIELD_LENGTH_TEMP_HEADER_LENGTH = 20
    };

//    enum TCP_OPTION{
//        TCP_OPTION_END_OF_OPTION = 0,
//        TCP_OPTION_NO_OPERATION = 1,
//        TCP_OPTION_MAXIMUM_SEGMENT_SIZE = 2,
//        TCP_OPTION_WINDOW_SCALE_OPTION = 3,
//        TCP_OPTION_SACK_PERMITTED_OPTION = 4,
//        TCP_OPTION_SACK_OPTION_FORMAT = 5,
//        TCP_OPTION_TIMESTAMPS_OPTION = 8
//    };

//    QHash<qint32,QString> tcp_option_val =
//    {
//        {TCP_OPTION_END_OF_OPTION,""}
//        ,{TCP_OPTION_NO_OPERATION,"No-operation"}
//        ,{TCP_OPTION_MAXIMUM_SEGMENT_SIZE,"Maximum segment size"}
//        ,{TCP_OPTION_WINDOW_SCALE_OPTION,"Window scale"}
//        ,{TCP_OPTION_SACK_PERMITTED_OPTION,"Sack permitted"}
//        ,{TCP_OPTION_SACK_OPTION_FORMAT,"SACK"}
//        ,{TCP_OPTION_TIMESTAMPS_OPTION,"Timestamps"}
//    };

    enum TRANSPORTLAYER_TCP_SERV{
        HTTP = 80
    };



//    enum TCP_OPTION_LENGTH:quint8{
//        TCP_OPTION_LENGTH_MAXIMUM_SEGMENT_SIZE = 4,
//        TCP_OPTION_LENGTH_WINDOW_SCALE_OPTION = 3,
//        TCP_OPTION_LENGTH_SACK_PERMITTED_OPTION = 2,
//        TCP_OPTION_LENGTH_SACK_OPTION_FORMAT = 8,
//        TRANSPORTLAYER_TCP_FIELD_LENGTH_TIMESTAMPS_OPTION = 10
//    };

    DissectResultTcp(DissectResultBase *dissectResultBase,void *reserves = nullptr);


//    void* GetNextLayer();

    /*处理端口号*/
    quint8* GetSourcePortPtr();
    quint16 GetSourcePort();
    quint8* GetDestinationPortPtr();
    quint16 GetDestinationPort();
    quint16 GetServPort();   //就是获取较小的端口号

    /*处理Seq和Ack*/
    quint32 GetSeq();
    quint32 GetRelativeSeq();
    quint32 GetAck();
    quint32 GetRelativeAck();

    /*实际就是获取首部长度，单位为四字节*/
    quint8 GetOffset();
    QString GetOffsetDotStr();
    quint32 GetPayloadLen();
    quint32 GetTotalLen();

    /*Flags*/
    quint8 GetReserved();
    QString GetReservedStr();
    bool RESERVED();
    bool NONCE();
    bool CWR();
    bool ECN_ECHO();
    bool URG();
    bool ACK();
    bool PSH();
    bool RST();
    bool SYN();
    bool FIN();
    QString GetFlagsStr();
    QString GetFlagMeanings();
    QString GetFlagDotMeanings();

    /*Window*/
    quint8* GetWindowPtr();
    quint16 GetWindow();
    quint32 GetCalculatedWindow();

    /*Checksum*/
    QString GetChecksumStr();
    QString GetCalculatedChecksumStr();

    /*Urgent Point*/
    quint16 GetUrgentPoint();

    /*Options*/
    /*
     *  QHash<qint32,struct {quint8 kind,quint8* startPtr}>
     */
//    qint32 GetOptionPtrByIndex(quint8 *kind,quint8 *length,const quint8 **ptr,qint32 index);
    qint32 GetOptionMaximumSegmentSize();
    qint16 GetOptionWindowScale(); //Window scale最大为14
    qint16 GetOptionWindowMultiplier();
    quint32 GetOptionTimestampValue();
    quint32 GetOptionTimestampEchoReply();
    qint8 GetOptionSackPermitted();
    QList<quint32> GetOptionSacks();
    QList<quint32> GetOptionRelativeSacks();

    qint32 GetOptionsCount();
    qint32 GetOptionTypeByIndex(qint32 index);
    qint32 GetOptionLenByIndex(qint32 index);
    qint32 GetOptionsLen();
    QString GetOptionsSummery();


//    TCP_OPTION_END_OF_OPTION = 0,
//    TCP_OPTION_NO_OPERATION = 1,
//    TCP_OPTION_MAXIMUM_SEGMENT_SIZE = 2,
//    TCP_OPTION_WINDOW_SCALE_OPTION = 3,
//    TCP_OPTION_SACK_PERMITTED_OPTION = 4,
//    TCP_OPTION_SACK_OPTION_FORMAT = 5,
//    TCP_OPTION_TIMESTAMPS_OPTION = 8

    /*分析Seq/Ack*/
    QString GetSegmentStatusStr();

    /*从DissectResultBase的保留字段获取数据，
    *取得分片的前一分片的包Index，若无前一个分片，或此分片长度为0，
    * 返回-1
    */
    qint64 GetPrevious();

    static Stream& GetStreamRecorder();
    static StreamTcp2& GetStreamTcp2Recorder();


private:
    /*0                   1          |        2                   3
     *0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
     *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
     *Source Port                    | Destination Port              |
     * ---------------------------------------------------------------
     *Sequence Number                                                |
     * ---------------------------------------------------------------
     *Acknowledgement Number                                         |
     * ---------------------------------------------------------------
     * Data  |Reserved   |U|A|P|R|S|F|   Window                      |
     * Offset|           |R|C|S|S|Y|I|                               |
     *       |           |G|K|H|T|N|N|                               |
     * ---------------------------------------------------------------
     * Checksum                      |     Urgent Point              |
     * ---------------------------------------------------------------
     * Options    .  Padding                                         |
     * ---------------------------------------------------------------
     * data ...                                                      |
     *----------------------------------------------------------------
     *
     */
    struct header_t{
        quint8 srcPort[TRANSPORTLAYER_TCP_FIELD_LENGTH_SOURCE_PORT];
        quint8 dstPort[TRANSPORTLAYER_TCP_FIELD_LENGTH_DESTINATION_PORT];
        quint8 seq[TRANSPORTLAYER_TCP_FIELD_LENGTH_SEQ];
        quint8 ack[TRANSPORTLAYER_TCP_FIELD_LENGTH_ACK];
        quint8 offset_reserved_flags[TRANSPORTLAYER_TCP_FIELD_LENGTH_DATAOFFSET_RESERVED_FLAGS];
        quint8 window[TRANSPORTLAYER_TCP_FIELD_LENGTH_WINDOW];
        quint8 checksum[TRANSPORTLAYER_TCP_FIELD_LENGTH_CHECKSUM];
        quint8 urgentPoint[TRANSPORTLAYER_TCP_FIELD_LENGTH_URGENTPOINT];
    };

    /*For Tcp Options*/
    /* |Kind | Length | Content |
     *  Length = Kind_len + Length_len + Content_len
     */
    struct option_dsc_t{
        const quint8 *ptr; /*内容部分的开始*/
        quint8 kind;
        quint8 length;
    };


    void addNextLayer(DissectResultBase *dissectResultBase,void *reserves = nullptr);

    /*Options*/
    /*
     *  QHash<qint32,struct {quint8 kind,quint8* startPtr}>
     */
    void dealTcpOptions();
    qint32 getOptionIndex(qint32 option);


    //static StreamTcp stream;

    QString calculatedChecksumStr;

    static StreamTcp2 stream2;

    struct header_t *header;

    //记录存在的Tcp选项
    QHash<qint32,struct option_dsc_t> options_dsc;
};

}
#endif // DISSECTRESULTTCP_H
