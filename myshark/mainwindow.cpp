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
    /*Font*/
    QFont font = this->ui->tableWidget->font();
    QFontMetrics fontMetrics(font);

    /*TableWidget*/
    this->scrollToBottom = false;
    this->ui->tableWidget->verticalHeader()->setVisible(false);
    this->ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableWidget->verticalHeader()->setMinimumSectionSize(0);
    this->ui->tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics.height());

    /*TreeWidget*/

    /*RawDataPanel*/
    this->ui->rawDataPanel->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->rawDataPanel->verticalHeader()->setVisible(true);
    this->ui->rawDataPanel->horizontalHeader()->hide();
    this->ui->rawDataPanel->setGridStyle(Qt::PenStyle::NoPen);

    this->ui->rawDataPanel->verticalHeader()->setMinimumSectionSize(0);
    this->ui->rawDataPanel->verticalHeader()->setDefaultSectionSize(fontMetrics.height());

    this->ui->rawDataPanel->setColumnCount(rawDataPanelColCount);
    this->ui->rawDataPanel->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    /*Display filter*/
    this->ui->lineEdit->setPlaceholderText("Display filter");

    /*StatuBar*/
    this->displayProportion = new QLabel();
    this->displayProportion->setLineWidth(5);
    this->ui->statusbar->addWidget(this->displayProportion);
}

void MainWindow::setupSignal(){
    //connect(ui->interfaceListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(StartCapture(QListWidgetItem *)));
    capturer = new Capturer(DeviceList::capHandle);
    dissector = new Dissector(capturer->GetDissResList(),capturer->GetIntLinkType());

    /*原来用于测试的，是暂时性的*/
    connect(this->capturer,SIGNAL(onePacketCaptured(qint64)),this->dissector,SLOT(Dissect(qint64)));
    connect(this->dissector,SIGNAL(onePacketDissected(qint64)),this,SLOT(Print(qint64)));

    /*安装事件过滤器*/
    this->ui->tableWidget->verticalScrollBar()->installEventFilter(this);

    connect(this->capturer,SIGNAL(onePacketCaptured(DissectResultFrame*)),this,SLOT(addToTable(DissectResultFrame*)));
}

void MainWindow::setTableWidgetColor(qint32 row,quint32 background, quint32 text){
    for(qint32 col = 0; col < this->tableWidgetColCount; col++){
        this->ui->tableWidget->item(row,col)->setBackground(QColor(background));
        this->ui->tableWidget->item(row,col)->setForeground(QColor(text));
    }
}

void MainWindow::setTableWidgetColor(qint32 row, quint32 background){
    for(qint32 col = 0; col < this->tableWidgetColCount; col++){
        this->ui->tableWidget->item(row,col)->setBackground(QColor(background));
    }
}

/*事件过滤器*/
bool MainWindow::eventFilter(QObject *target, QEvent *event){
    if (target == this->ui->tableWidget->verticalScrollBar()){
        if(event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheelEvent = (QWheelEvent*)event;
            if (wheelEvent->delta() > 0) {
                this->scrollToBottom  = false;
                this->ui->actionScrollToLastLine->setChecked(false);
            }
        }
    }
    return QMainWindow::eventFilter(target, event);
}

/*用于测试的，暂时性的*/
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

///*用于测试的，暂时性的*/
//void MainWindow::StartCapture(QListWidgetItem *item){
//    //ui->interfaceListWidget->hide();
//    this->capturer = new Capturer(item->text());
//    this->dissector = new Dissector(capturer->GetDissResList(),capturer->GetIntLinkType());
//    connect(this->capturer,SIGNAL(onePacketCaptured(qint64)),this->dissector,SLOT(Dissect(qint64)));
//    connect(this->dissector,SIGNAL(onePacketDissected(qint64)),this,SLOT(Print(qint64)));
//    this->capturer->start();

//    qDebug() << "选中的DeviceName: " << item->text();
//}

/*用于测试的，暂时性的*/
void MainWindow::Print(qint64 index){
    //测试DissectResultFrame
    DissectResultFrame *frame = this->capturer->GetDissectResultFrameByIndex(index);
//    qDebug() << "Frame->"
//             << "Index:" <<frame->GetIndex()
//             << "Time:" << QString::asprintf("%.9f",frame->GetRelativeTimeSinceFirstPacket())
//             << "CapLen" << frame->GetCapLen();

    //测试DissectResultLinklayer
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

    //测试DissectResultIpv4
    if( *(quint16*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypePtr() == tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_TYPE_IPV4){
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
        if(ipv4->GetType() == tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_TYPE_TCP){
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
                 << "max-segment:" << tcp->GetOptionMaximumSegmentSize()
                    ;
        }else{
            //qDebug() << "未处理的运输层协议，协议号为" << ipv4->GetType();
        }
    }else{
        //qDebug() << "未处理的网络层协议，协议号为" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeStr();
    }

qDebug() << "----------------------------------------------------------------------------------------------------------------------------------------------";

//    Info *info = new Info(0,1);  //devIndex , linktype
//    ProTree *proTree = this->dissector->GetLoader()->GetDissector(capturer->GetIntLinkType())->Dissect(
//                this->dissector->GetDissResList()
//                ,index
//                ,info
//                );
//    this->PrintProTree(proTree->GetHeader());
//    delete info;

}

void MainWindow::addToTable(DissectResultFrame *frame){
    QTableWidgetItem *item;
    QString str;
    qint32 row = this->ui->tableWidget->rowCount();

    this->ui->tableWidget->insertRow(row);

    //No.
    item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole,frame->GetIndex());
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
    item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole,frame->GetCapLen());
    this->ui->tableWidget->setItem(row,MainWindow::COL_LENGTH,item);

    //Summery
    str.clear();
    str.append(frame->GetSummery());
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_INFO,item);

    //背景色
    if( frame->GetDissectResultBase()->GetAdditionalVal(TCP_STATUS) != -1){
        this->setTableWidgetColor(row,0x3C3C3C,0xff00cc);
    }else if(frame->GetDissectResultBase()->GetAdditionalVal(TCP_ISSYN) == 1){
        this->setTableWidgetColor(row,0xAAAAAA);
    }else if(frame->GetDissectResultBase()->GetAdditionalVal(TCP_ISRST) == 1){
        this->setTableWidgetColor(row,0xA21919);
    }else{
        this->setTableWidgetColor(row,0xD5D5D5);
    }


    if(this->scrollToBottom)
        this->ui->tableWidget->scrollToBottom();

    this->displayProportion->clear();
    this->displayProportion->setText(QString("%1%").arg(this->ui->tableWidget->rowCount()*1.0/this->capturer->GetCount()*100));
}

void MainWindow::ergoditTree(QTreeWidgetItem *parent,ProTreeNode *node){
    QTreeWidgetItem *item = nullptr;
    while (node != nullptr) {
        item = new QTreeWidgetItem();
        item->setText(0,node->GetMsg());
        if(parent != nullptr)
            parent->addChild(item);
        else
            this->ui->treeWidget->addTopLevelItem(item);
        if(node->GetNextLevel() != nullptr)
            this->ergoditTree(item,node->GetNextLevel());
        node = node->GetNext();
    }
}

void MainWindow::addToTree(qint64 index){
     ProTreeMaker *maker = new ProTreeMaker(this->capturer->GetCapHandle()->GetLinkType(),this->capturer->GetDissectResultFrameByIndex(index),nullptr);
     ProTreeNode *node = maker->GetProTree()->GetHeader();
     this->ui->treeWidget->clear();
     this->ergoditTree(nullptr,node);
}


void MainWindow::addToRawDataPanel(qint64 index){
    const quint8* ptr = this->capturer->GetDissectResultFrameByIndex(index)->GetData();
    const quint8* line = nullptr;
    qint32 capLen = this->capturer->GetDissectResultFrameByIndex(index)->GetCapLen();
    QString text;
    qint32 rowCount = 0;

    this->ui->rawDataPanel->clearContents();
    this->ui->rawDataPanel->setRowCount(0);
    for(qint32 i = 0; i < capLen; i++){
        if(i % 16 == 0){
            this->ui->rawDataPanel->insertRow(rowCount);
            this->ui->rawDataPanel->setVerticalHeaderItem(rowCount, new QTableWidgetItem(QString::asprintf("%04x",rowCount)));
            rowCount++;
        }
    }

    for(qint32 row =0; row < rowCount; row++){
        line = ptr + row * 16;
        for(qint32 col = 0; col < rawDataPanelColCount; col++){
            text.clear();
            if( col == 8 || col == 17 ){
                ;
            }else if( col < 8 ){
                if( row * 16 + col + 1 > capLen )
                    continue;
                text.append(QString::asprintf("%02x",line[col]));
            }else if( col < 17){
                if( row * 16 + col > capLen )
                    continue;
                text.append(QString::asprintf("%02x",line[col - 1]));
            }else{
                if( row * 16 + col - 17 > capLen )
                    break;
                if(isprint(line[col - 18]))
                    text.append(QString::asprintf("%c",line[col - 18]));
                else
                    text.append(QString::asprintf("."));
            }
            this->ui->rawDataPanel->setItem(row,col,new QTableWidgetItem(text));
        }
    }

}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column)
    qint64 index = this->ui->tableWidget->item(row,COL_NO)->text().toULongLong();
    this->addToRawDataPanel(index);
    this->addToTree(index);
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
    this->ui->tableWidget->clearContents();
    this->ui->tableWidget->setRowCount(0);
    this->ui->treeWidget->clear();
    this->ui->rawDataPanel->clearContents();
    this->ui->rawDataPanel->setRowCount(0);
    this->ui->actionStart->setEnabled(false);
    this->ui->actionStop->setEnabled(true);
    this->ui->actionRestart->setEnabled(true);
    this->capturer->start();
}

void MainWindow::on_actionRestart_triggered()
{

}


/*控制MainWindow的字体大小*/
void MainWindow::on_actionEnlargeTextSize_triggered()
{
    QFont font(this->ui->tableWidget->font());
    font.setPointSize(font.pointSize() + 1);
    QFontMetrics fontMetrics(font);

    this->ui->tableWidget->setFont(font);
    this->ui->tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics.height());

    this->ui->treeWidget->setFont(font);

    this->ui->rawDataPanel->setFont(font);
    this->ui->rawDataPanel->verticalHeader()->setDefaultSectionSize(fontMetrics.height());
}

void MainWindow::on_actionShrinkTextSize_triggered()
{
    QFont font(this->ui->tableWidget->font());
    if(font.pointSize() > 1)
        font.setPointSize(font.pointSize() - 1);
    QFontMetrics fontMetrics(font);

    this->ui->tableWidget->setFont(font);
    this->ui->tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics.height());

    this->ui->treeWidget->setFont(font);

    this->ui->rawDataPanel->setFont(font);
    this->ui->rawDataPanel->verticalHeader()->setDefaultSectionSize(fontMetrics.height());
}

void MainWindow::on_actionDefaultTextSize_triggered()
{
    QFont font(this->ui->tableWidget->font());
    font.setPointSize(this->defaultTextSize);
    QFontMetrics fontMetrics(font);

    QFont equalWidthFont(this->ui->rawDataPanel->font());
    equalWidthFont.setPointSize(this->defaultTextSize);
    QFontMetrics equalWidthFontMetrics(equalWidthFont);

    this->ui->tableWidget->setFont(font);
    this->ui->tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics.height());

    this->ui->treeWidget->setFont(font);

    this->ui->rawDataPanel->setFont(equalWidthFont);
    this->ui->rawDataPanel->verticalHeader()->setDefaultSectionSize(equalWidthFontMetrics.height());
    for(qint32 index = 0; index < rawDataPanelColCount; index++){
        if( index < 18 )
            this->ui->rawDataPanel->horizontalHeader()->resizeSection(index,equalWidthFont.pointSize() * 2);
        else
            this->ui->rawDataPanel->horizontalHeader()->resizeSection(index,equalWidthFont.pointSize());
    }
}

void MainWindow::on_actionScrollToLastLine_triggered(bool checked)
{
    this->scrollToBottom = checked;
    if(checked)
        this->ui->tableWidget->scrollToBottom();
}

void MainWindow::on_actionResizeTableWidgetTOFitContents_triggered()
{
    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
