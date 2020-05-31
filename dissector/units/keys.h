#ifndef KEYS_H
#define KEYS_H

#include <QString>

/*需要添加到DissectResultBase的保留字段的值的Key*/

/*Common*/
const QString DISSECT_RESULT_BASE_LIST  = "dissect_result_base_list";


/*For Tcp*/
//const QString TCP_INFO = "tcp_info";
const QString TCP_INFO_PTR = "tcp_info_ptr";
//const QString TCP_STREAM = "tcp_stream";

/*For Ipv4*/
//const QString IPV4_INFO;
const QString IPV4_INFO_PTR;

/*For UDP*/
const QString UDP_INFO_PTR = "udp_info_ptr";

//const QString TCP_SEQ_VAL = "tcp_seq_val";
//const QString TCP_ACK_VAL = "tcp_ack_val";
//const QString TCP_PAYLOAD_LEN = "tcp_payload_len";
//const QString TCP_TOTAL_LEN = "tcp_total_len";
//const QString TCP_ISURG = "tcp_isurg";
//const QString TCP_ISACK = "tcp_isack";
//const QString TCP_ISPSH = "tcp_ispsh";
//const QString TCP_ISRST = "tcp_isrst";
//const QString TCP_ISFIN = "tcp_isfin";
//const QString TCP_ISSYN = "tcp_issyn";
//const QString TCP_WINDOW = "tcp_window";
//const QString TCP_WINDOW_SCALE = "tcp_window_scale";
//const QString TCP_WINDOW_MULTIPLIER = "tcp_window_multiplier";
//const QString TCP_DUPACK_NUM = "tcp_dupack_num";
//const QString TCP_DUPACK_FRAME = "tcp_dupack_frame";


//const QString TCP_PRE_SEGMENT = "tcp_pre_seqment";
//const QString TCP_STATUS = "tcp_status";
//const QString TCP_STATUS2 = "tcp_status2";
//const QString TCP_VALIED_DATA_START_PTR ="tcp_valied_data_start_ptr";


//const QString IP_SOURCE_ADDRESS_PTR = "ip_source_address_ptr";
//const QString IP_DESTINATION_ADDRESS_PTR = "ip_destination_address_ptr";
//const QString IP_ADDRESS_LENGTH = "ip_address_length";
//const QString IP_PERSUDO_HEADER_PTR = "ip_persudo_header_ptr";
//const QString IP_PERSUDO_HEADER_LENGTH = "ip_persudo_header_length";

//const QString MAC_FRAME_HAVE_FCS = "mac_frame_have_fcs";

#endif // KEYS_H
