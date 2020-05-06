#include "linklayer.h"

Linklayer::Linklayer(ProTree *proTree,tcp_ip_protocol_family::DissectResultLinkLayer *dissectResultLinklayer,void *reserves)
{
    QString linklayerTypeName(DissectResultBase::GetLinklayerTypeName());
    quint64 dissectorOption = DissectResultBase::GetDissectorOptionPtr()->value("ether");
    Q_UNUSED(dissectorOption)
    Q_UNUSED(reserves)
        /*LinkLayer处理代码*/
        Q_UNUSED(dissectResultLinklayer)
//        proTree->AddItem("frame","linklayer summery"); // -
//        proTree->AddItem("frame","1.1",1); // - -
//        proTree->AddItem("frame","1.2",0); // - -

//        proTree->Pop(1);
    qint32 start = dissectResultLinklayer->GetDissectResultBase()->GetProtocolHeaderStartPositionByName("ether");
    // Summery
    proTree->AddItem("ether",
                     QString("%1, Src: %2 ( %3 ), Dst: %4 ( %5 )")
                     .arg(linklayerTypeName)
                     .arg(dissectResultLinklayer->GetSourceAddressStr())
                     .arg(dissectResultLinklayer->GetSourceAddressOriginalStr())
                     .arg(dissectResultLinklayer->GetDestinationAddressStr())
                     .arg(dissectResultLinklayer->GetDestinationAddressOriginalStr())
                     ,start
                     ,dissectResultLinklayer->GetDissectResultBase()->GetProtocolHeaderEndPositionByName("ether") - start - 1
                     );

    // Summery - Destination
    proTree->AddNextLayerItem("ether",
                     QString("Destination: %1 ( %2 )")
                     .arg(dissectResultLinklayer->GetDestinationAddressStr())
                     .arg(dissectResultLinklayer->GetDestinationAddressOriginalStr())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_DES_ADDR
                     );

    // Summery - Destination - Address
    proTree->AddNextLayerItem("ether",
                     QString("Address: %1 ( %2 )")
                     .arg(dissectResultLinklayer->GetDestinationAddressStr())
                     .arg(dissectResultLinklayer->GetDestinationAddressOriginalStr())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_DES_ADDR
                     );

    // Summery - Destination - LG bit
    proTree->AddCurrentLayerItem("ether",
                     QString("%1 = LG bit: %2 ( %3 )")
                     .arg(dissectResultLinklayer->GetDestinationAddressLGBitStr())
                     .arg(dissectResultLinklayer->GetDestinationAddressLGBit_meaning())
                     .arg(dissectResultLinklayer->GetDestinationAddressLGBit_short_meaning())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_DES_ADDR
                     );

    // Summery - Destination - IG bit
    proTree->AddCurrentLayerItem("ether",
                     QString("%1 = IG bit: %2 ( %3 )")
                     .arg(dissectResultLinklayer->GetDestinationAddressIGBitStr())
                     .arg(dissectResultLinklayer->GetDestinationAddressIGBit_meaning())
                     .arg(dissectResultLinklayer->GetDestinationAddressIGBit_short_meaning())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_DES_ADDR
                     ,true
                     );


    // Summery - Source
    proTree->Pop();
    proTree->AddCurrentLayerItem("ether",
                     QString("Source: %1 ( %2 )")
                     .arg(dissectResultLinklayer->GetSourceAddressStr())
                     .arg(dissectResultLinklayer->GetSourceAddressOriginalStr())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_SRC_ADDR
                     );

    // Summery - Source - Address
    proTree->AddNextLayerItem("ether",
                     QString("Address: %1 ( %2 )")
                     .arg(dissectResultLinklayer->GetSourceAddressStr())
                     .arg(dissectResultLinklayer->GetSourceAddressOriginalStr())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_SRC_ADDR
                     );

    // Summery - Source - LG bit
    proTree->AddCurrentLayerItem("ether",
                     QString("%1 = LG bit: %2 ( %3 )")
                     .arg(dissectResultLinklayer->GetSourceAddressLGBitStr())
                     .arg(dissectResultLinklayer->GetSourceAddressLGBit_meaning())
                     .arg(dissectResultLinklayer->GetSourceAddressLGBit_short_meaning())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_SRC_ADDR
                     );

    // Summery - Source - IG bit
    proTree->AddCurrentLayerItem("ether",
                     QString("%1 = IG bit: %2 ( %3 )")
                     .arg(dissectResultLinklayer->GetSourceAddressIGBitStr())
                     .arg(dissectResultLinklayer->GetSourceAddressIGBit_meaning())
                     .arg(dissectResultLinklayer->GetSourceAddressIGBit_short_meaning())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_SRC_ADDR
                     ,true
                     );

    // Summery - Type
    proTree->Pop();
    proTree->AddCurrentLayerItem("ether",
                     QString("Type: %1 ( %2 )")
                     .arg(dissectResultLinklayer->GetTypeName())
                     .arg(dissectResultLinklayer->GetTypeStr())
                     ,&start
                     ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_TYPE
                     ,true
                     );

//    //测试解析选项
//    proTree->AddCurrentLayerItem("ether",
//                                 QString("Validate ethernet fcs ? %1")
//                                 .arg(((dissectorOption & ETHERNET_VALIDATE_FCS) ? "YES":"NO"))
//                                 ,&start
//                                 ,tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_FIELD_LENGTH_TYPE
//                                 ,false
//                                 );

    // Summery
    proTree->Pop();
}
