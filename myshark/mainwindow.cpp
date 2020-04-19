#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../dissector/dissres/dissectresutltframe.h"
#include "../dissector/dissres/tcp_ip_protocol_family/dissectresultlinklayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Device device;
    capturer = new Capturer(Device::GetDeviceNameByIndex(0));

    dissector = new Dissector(capturer->GetDissResList(),capturer->GetIntLinkType());

    connect(this->capturer,SIGNAL(onePacketCaptured(qint64)),this->dissector,SLOT(Dissect(qint64)));
    connect(this->dissector,SIGNAL(onePacketDissected(qint64)),this,SLOT(Print(qint64)));
    capturer->Start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::PrintProTree(ProTreeNode *proTreeNode, qint32 level){
    while (proTreeNode != NULL){
        QString str;
        qint32 index = 0;
        while (index < level) {
            if(level == 1)
                str += "@";
            else
                str += "=";
            index++;
        }
        qDebug() << str << proTreeNode->GetMsg() << proTreeNode->GetStart() << "~" << proTreeNode->GetEnd();
        if( proTreeNode->GetNextLevel() != NULL)
            PrintProTree(proTreeNode->GetNextLevel(),level + 5);
        proTreeNode = proTreeNode->GetNext();
    }
}

void MainWindow::Print(qint64 index){
//    DissResEth *res = (DissResEth*)(this->dissector->GetDissResByIndex(index));
//    qDebug() << res->GetNo()
//             << ""  << res->GetTimeSinceFirstPacket()
//             << ""  << res->GetTopProtocol()
//             << ""  << res->GetStrSrc()
//             << ""  << res->GetStrDst()
//             << ""  << res->GetCapLen()
//             << ""  << res->GetTopProtocol()
//             << ""  << res->GetSrcPort()
//             << ""  << res->GetDstPort()
//             << ""  << res->GetMsg();
    DissectResultFrame *frame = this->capturer->testList.at(index);
//    qDebug() << "Frame->"
//             << "Index:" <<frame->GetIndex()
//             << "Time:" << QString::asprintf("%.9f",frame->GetRelativeTimeSinceFirstPacket())
//             << "CapLen" << frame->GetCapLen();

//    qDebug() << "LinkLayer->"
//             << "Stream:" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetStreamIndex()
//             << "o-Stream:" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetOriginalStreamIndex()
//             << "Addr:"<< frame->GetTcpIpProtocolFamilyBaseLayer()->GetSourceAddressStr()
//             << frame->GetTcpIpProtocolFamilyBaseLayer()->GetDestinationAddressStr()
//             << "Type:" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeStr()
//             << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeName()
//             << "I/G:" << (frame->GetTcpIpProtocolFamilyBaseLayer()->SourceAddressIsGroup() ? "Group":"Individual")
//             << (frame->GetTcpIpProtocolFamilyBaseLayer()->DestinationAddressIsGroup() ? "Group":"Individual")
//             << "G/L:" << (frame->GetTcpIpProtocolFamilyBaseLayer()->SourceAddressIsLocalAdministered() ? "Local":"Global")
//             << (frame->GetTcpIpProtocolFamilyBaseLayer()->DestinationAddressIsLocalAdministered() ? "Local":"Global")
//             << "HaveFCS:" << (frame->GetTcpIpProtocolFamilyBaseLayer()->HaveFCS()?"Yes":"No")
//             << "FCS:" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetFCSStr()
//             << "c-FCS:" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetCalculatedFCSStr()
             ;

    switch (*(quint16*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypePtr()) {
    case tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_TYPE_IPV4:
    {
        tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer();
//        qDebug() << "Ipv4->"
//                 << "Stream:" << ipv4->GetStreamIndex()
//                 << "o-Stream:" << ipv4->GetOriginalStreamIndex()
//                 << "Version:"<< ipv4->GetVersion()
//                 << "Adr:" << ipv4->GetSourceAddressStr()
//                 << ipv4->GetDestinationAddressStr()
//                 << "Type:" << ipv4->GetType() << ipv4->GetTypeName()
//                 << "Ceksum:" << ipv4->GetChecksumStr()
//                 << "ECN:" << ipv4->GetDSField_ECN_meanning()
//                 << "DSCP:" << ipv4->GetDSField_DSCP_meanning()
//                 << "Flag-Offset" << ipv4->GetFlagOffsetStr()
//                 << "DF:" << ipv4->DF_meaning()
//                 << "MF:" << ipv4->MF_meaning()
//                 << "Fragment:" << ipv4->GetFragmentOffset()
//                 ;

        switch (ipv4->GetType()) {
        case tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_TYPE_TCP:
        {
        tcp_ip_protocol_family::DissectResultTcp *tcp = (tcp_ip_protocol_family::DissectResultTcp*)ipv4->GetNextLayer();
        qDebug() << "Tcp->"
                 << "index:" << frame->GetIndex()
                 << "o-stream:" << tcp->GetOriginalStreamIndex()
                 << "srcPort:" << tcp->GetSourcePort()
                 << "dstPort:" << tcp->GetDestinationPort()
                 << (tcp->SYN()?"SYN":"")
                 << "seq:" << tcp->GetSeq()
                 << "r-seq:" << tcp->GetRelativeSeq()
                 << "len:" << tcp->GetPayloadLen()
                 << "pre:" <<((tcp->GetPrevious() == -1)?"无":QString("%1").arg(tcp->GetPrevious()))
                 << "status:" << tcp->GetSegmentStatusStr()
                    ;
         qDebug() << "----------------------------------------------------------------------------------------------------------------------------------------------";
        break;
        }
        default:
//            qDebug() << "未处理->"
//                     << "还没有处理的协议"
                     ;
        }

        break;
    }
    default:
//        qDebug() << "未处理->"
//                 << "还没有处理的协议"
//                 << "协议号为" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeStr()
//                 << "协议名为" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeName()
                    ;

    }

   // qDebug() << "----------------------------------------------------------------------------------------------------------------------------------------------";

//    Info *info = new Info(0,1);  //devIndex , linktype
//    ProTree *proTree = this->dissector->GetLoader()->GetDissector(capturer->GetIntLinkType())->Dissect(
//                this->dissector->GetDissResList()
//                ,index
//                ,info
//                );
//    this->PrintProTree(proTree->GetHeader());
//    delete info;

//    qint32 i;
//    for(i = 0; i < res->GetCapLen() - 5; i+= 6)
//        qDebug() << QString::asprintf("%02x %02x %02x %02x %02x %02x",res->GetData()[i]
//                                                                ,res->GetData()[i + 1]
//                                                                ,res->GetData()[i + 2]
//                                                                ,res->GetData()[i + 3]
//                                                                ,res->GetData()[i + 4]
//                                                                ,res->GetData()[i + 5]);
//    while (i < res->GetCapLen()) {
//        qDebug() << QString::asprintf("%02x",res->GetData()[i]) <<  " ";
//        i++;
//    }

}

