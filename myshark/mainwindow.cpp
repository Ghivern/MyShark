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
    qDebug() << "DissectResultFrame"
             << "Index:" <<frame->GetIndex()
             << "Time:" << frame->GetRelativeTimeSinceFirstPacket()
             << "CapLen" << frame->GetCapLen();

    qDebug() << "DissectResultLinkLayer"
             << "Addr:"<< frame->GetTcpIpProtocolFamilyBaseLayer()->GetSourceAddressStr()
             << frame->GetTcpIpProtocolFamilyBaseLayer()->GetDestinationAddressStr()
             << "Type:" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeStr()
             << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeName()
             << "I/G:" << (frame->GetTcpIpProtocolFamilyBaseLayer()->SourceAddressIsGroup() ? "Group":"Individual")
             << (frame->GetTcpIpProtocolFamilyBaseLayer()->DestinationAddressIsGroup() ? "Group":"Individual")
             << "G/L:" << (frame->GetTcpIpProtocolFamilyBaseLayer()->SourceAddressIsLocalAdministered() ? "Local":"Global")
             << (frame->GetTcpIpProtocolFamilyBaseLayer()->DestinationAddressIsLocalAdministered() ? "Local":"Global")
             ;

    switch (*(quint16*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypePtr()) {
    case tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_TYPE_IPV4:
    {
        tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer();
        qDebug() << "DissectResultIpv4"
                 << "Version:"<< ipv4->GetVersion()
                 << "Adr:" << ipv4->GetSourceAddressStr()
                 << ipv4->GetDestinationAddressStr()
                 << "Type:" << ipv4->GetType() << ipv4->GetTypeName()
                 << "Ceksum:" << ipv4->GetChecksumStr();
        break;
    }
    default:
        qDebug() << "DissectResultIpv4"
                 << "还没有处理的协议"
                 << "协议号为" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeStr()
                 << "协议名为" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeName();

    }

    qDebug() << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-";

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

