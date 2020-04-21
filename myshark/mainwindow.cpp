#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../dissector/dissres/dissectresutltframe.h"
#include "../dissector/dissres/tcp_ip_protocol_family/dissectresultlinklayer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setupUi();
    this->setupSignal();

    this->setWindowTitle(DeviceList::SelectedDevice);
    this->ui->actionStart->setEnabled(false);
    capturer->Start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi(){
    this->scrollToBottom = false;
    this->ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->ui->lineEdit->setPlaceholderText("Display filter");

    this->status = new QLabel("---------------------");
    this->ui->statusbar->addWidget(status);
}

void MainWindow::setupSignal(){
    //connect(ui->interfaceListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(StartCapture(QListWidgetItem *)));
    capturer = new Capturer(DeviceList::capHandle);
    dissector = new Dissector(capturer->GetDissResList(),capturer->GetIntLinkType());

    connect(this->capturer,SIGNAL(onePacketCaptured(qint64)),this->dissector,SLOT(Dissect(qint64)));
    connect(this->dissector,SIGNAL(onePacketDissected(qint64)),this,SLOT(Print(qint64)));


    connect(this->capturer,SIGNAL(onePacketCaptured(DissectResultFrame*)),this,SLOT(addToTable(DissectResultFrame*)));
}


void MainWindow::addToTable(DissectResultFrame *frame){
    QTableWidgetItem *item;
    QString str;
    qint32 row = this->ui->tableWidget->rowCount();

    this->ui->tableWidget->insertRow(row);

    //No
    str.append(QString("%1").arg(frame->GetIndex()));
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_NO,item);

    //Time
    str.clear();
    str.append(QString::asprintf("%.8lf",frame->GetRelativeTimeSinceFirstPacket()));
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_TIME,item);


    //Source
    str.clear();
    if(frame->ContainProtocol("ipv4")){
        tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer();
        str.append(ipv4->GetSourceAddressStr());
    }else{
        tcp_ip_protocol_family::DissectResultLinkLayer *linklayer = frame->GetTcpIpProtocolFamilyBaseLayer();
        str.append(linklayer->GetSourceAddressStr());
    }
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_SOURCE,item);

    //Destination
    str.clear();
    if(frame->ContainProtocol("ipv4")){
        tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer();
        str.append(ipv4->GetDestinationAddressStr());
    }else{
        tcp_ip_protocol_family::DissectResultLinkLayer *linklayer = frame->GetTcpIpProtocolFamilyBaseLayer();
        str.append(linklayer->GetDestinationAddressStr());
    }
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_DESTINATION,item);

    //Protocol
    str.clear();
    str.append(frame->GetTopProtocolName());
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_PROTOCOL,item);

    //Length
    str.clear();
    str.append(QString("%1").arg(frame->GetCapLen()));
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_LENGTH,item);

    //Summery
    str.clear();
    str.append(frame->GetSummery());
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_INFO,item);

    if(this->scrollToBottom)
        this->ui->tableWidget->scrollToBottom();

    this->status->clear();
    this->status->setText(QString("%1%").arg(this->ui->tableWidget->rowCount()*1.0/this->capturer->GetCount()*100));
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

void MainWindow::StartCapture(QListWidgetItem *item){
    //ui->interfaceListWidget->hide();
    this->capturer = new Capturer(item->text());
    this->dissector = new Dissector(capturer->GetDissResList(),capturer->GetIntLinkType());
    connect(this->capturer,SIGNAL(onePacketCaptured(qint64)),this->dissector,SLOT(Dissect(qint64)));
    connect(this->dissector,SIGNAL(onePacketDissected(qint64)),this,SLOT(Print(qint64)));
    this->capturer->start();

    qDebug() << "选中的DeviceName: " << item->text();
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
                 //<< "ack:" << tcp->GetAck()
                 //<< "r-ack:" << tcp->GetRelativeAck()
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


void MainWindow::on_actionStop_triggered()
{
    this->ui->actionStart->setEnabled(true);
    this->ui->actionStop->setEnabled(false);
    this->ui->actionRestart->setEnabled(false);
    this->capturer->Stop();
}

void MainWindow::on_actionStart_triggered()
{
    for(qint32 index = this->ui->tableWidget->rowCount() - 1; index >=0; index--)
        this->ui->tableWidget->removeRow(index);
    this->ui->treeWidget->clear();
    this->ui->textEdit->clear();
    this->ui->actionStart->setEnabled(false);
    this->ui->actionStop->setEnabled(true);
    this->ui->actionRestart->setEnabled(true);
    this->capturer->start();
}

void MainWindow::on_actionRestart_triggered()
{
    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->scrollToBottom = true;
}
