#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../dissector/dissres/dissectresutltframe.h"
//#include "../dissector/dissres/tcp_ip_protocol_family/dissectresultlinklayer.h"

MainWindow::MainWindow(QHash<QString,quint64> *dissectorOptions,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->streamIndex = -1;
    this->capturer = nullptr;
    this->haveDate = false;

    this->setupUi();

    this->dissectorOptions = dissectorOptions; //必须在setupSignal前面
    this->setupSignal();

//    this->ui->widget_2->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi(){
    this->ui->tableWidget->hide();
    this->ui->treeWidget->hide();
    this->ui->rawDataPanel->hide();

    /*Device List*/
    this->ui->listWidget->setStyleSheet("background-color:transparent");
    Device device;
    QListWidgetItem *item;
    for(qint32 index = 0; index < device.GetDeviceCount(); index++){
        item = new QListWidgetItem();
        item->setText(device.GetDevices().at(index));
//        item->setText("hello");
        this->ui->listWidget->addItem(item);
    }
    this->ui->listWidget->setCurrentRow(0);
    this->selectedDevName.clear();
    this->selectedDevName.append(this->ui->listWidget->item(0)->text());


    /*MainWindow*/
    this->resize(1050,650);
    this->setWindowTitle("MyShark");

    /*Actions*/
    this->ui->actionStart->setEnabled(false);
    this->ui->actionDissector_options->setEnabled(false);

    /*Font*/
    QFont font = this->ui->tableWidget->font();
    font.setPointSize(this->defaultTextSize);
    QFontMetrics fontMetrics(font);

    /*TableWidget*/
    this->scrollToBottom = false;
    this->ui->tableWidget->setFont(font);
    this->ui->tableWidget->verticalHeader()->setVisible(false);
    this->ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    this->ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableWidget->verticalHeader()->setMinimumSectionSize(0);
    this->ui->tableWidget->verticalHeader()->setDefaultSectionSize(fontMetrics.height());
    this->ui->tableWidget->setColumnWidth(tableWidgetColCount - 1,1000);

    /*TreeWidget*/

    /*RawDataPanel*/
    this->ui->rawDataPanel->setFont(font);
    this->ui->treeWidget->setFont(font);
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

    /*Dialog*/
    this->saveOrCloseFileDialog = new SaveOrCloseFileDialog;

    this->ui->actionStop->setEnabled(false);
    this->ui->actionStart->setEnabled(true);
    this->ui->actionRestart->setEnabled(false);
    this->ui->actionDissector_options->setEnabled(true);
}

void MainWindow::setupSignal(){
    //connect(ui->interfaceListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(StartCapture(QListWidgetItem *)));
//    capturer = new Capturer(DeviceList::capHandle,this->dissectorOptions);

    /*安装事件过滤器*/
    this->ui->tableWidget->verticalScrollBar()->installEventFilter(this);

    connect(this->saveOrCloseFileDialog,SIGNAL(continueWithoutSave()),this,SLOT(slot_startCapture()));
    connect(this->saveOrCloseFileDialog,SIGNAL(saveFileBeforeCapture(QString)),this,SLOT(slot_saveFileBeforeCapture(QString)));

//    connect(this,SIGNAL(signal_startCapture()),this,SLOT(slot_startCapture()));
//    emit signal_startCapture();
//    try {
        //DeviceList::capHandle->ActivateHandleWithParas();
//        this->capturer = new Capturer(DeviceList::capHandle,this->dissectorOptions);
//        connect(this->capturer,SIGNAL(onePacketCaptured(DissectResultFrame*)),this,SLOT(addToTable(DissectResultFrame*)));
//        capturer->Start();
//    } catch (QString e) {
//        this->capturer = nullptr;
//        if( this->capturer != nullptr ){
//            this->capturer->deleteLater();
//            this->capturer = nullptr;
//        }
//        this->ui->actionStop->setEnabled(false);
//        this->ui->actionStart->setEnabled(true);
//        this->ui->actionRestart->setEnabled(false);
//        this->ui->actionDissector_options->setEnabled(true);
//        QMessageBox::critical(this,"Error",e);
//        return;
//    }

    //dissector = new Dissector(capturer->GetDissResList(),capturer->GetIntLinkType());

    /*原来用于测试的，是暂时性的*/
    //connect(this->capturer,SIGNAL(onePacketCaptured(qint64)),this->dissector,SLOT(Dissect(qint64)));
    //connect(this->dissector,SIGNAL(onePacketDissected(qint64)),this,SLOT(Print(qint64)));



    //connect(this->capturer,&QThread::finished,this->capturer,&QThread::deleteLater);


}

/*初始化解析选项*/
void MainWindow::setupDissectorOptions(){
    this->dissectorOptions = new QHash<QString,quint64>;
//    this->dissectorOptions->insert("tcp",5); //测试
}

void MainWindow::setTableWidgetColor(qint32 row,quint32 background, quint32 textColor){
    for(qint32 col = 0; col < this->tableWidgetColCount; col++){
        if( this->ui->tableWidget->item(row,col) ){
            this->ui->tableWidget->item(row,col)->setBackground(QColor(background));
            this->ui->tableWidget->item(row,col)->setForeground(QColor(textColor));
        }
    }
}

void MainWindow::setTableWidgetColor(qint32 row, quint32 background){
    for(qint32 col = 0; col < this->tableWidgetColCount; col++){
        if( this->ui->tableWidget->item(row,col) ){
            this->ui->tableWidget->item(row,col)->setBackground(QColor(background));
        }
    }
}

void MainWindow::clearRawDataPanelBackground(quint32 background){
    qint32 row,col;
    for( row = 0; row < this->ui->rawDataPanel->rowCount() - 1; row++ ){
        for( col = 0; col < this->ui->rawDataPanel->columnCount(); col++ ){
            if( col != 17 )
                this->ui->rawDataPanel->item(row,col)->setData(Qt::BackgroundRole,QColor(background));
        }
    }

    for( col = 0; col < this->ui->rawDataPanel->columnCount(); col++ ){
        if( !this->ui->rawDataPanel->item(row,col)->text().isEmpty() ){
            this->ui->rawDataPanel->item(row,col)->setData(Qt::BackgroundRole,QColor(background));
        }
    }

    if( !this->ui->rawDataPanel->item(row,9)->text().isEmpty() ){
        this->ui->rawDataPanel->item(row,8)->setData(Qt::BackgroundRole,QColor(background));
    }
}

void MainWindow::addBackgroundToTableRow(DissectResultFrame *frame,qint32 row){
    if( frame->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR) ){
        TcpInfo *tcpInfo = (TcpInfo*)frame->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR);
        if( tcpInfo->status ){
            this->setTableWidgetColor(row,0x3C3C3C,0xff00cc);
        }else if( tcpInfo->SYN || tcpInfo->FIN ){
            this->setTableWidgetColor(row,0xAAAAAA);
        }else if( tcpInfo->RST ){
            this->setTableWidgetColor(row,0xA21919);
        }else{
            this->setTableWidgetColor(row,0xD5D5D5);
        }
    }else{
        this->setTableWidgetColor(row,0xD5D5D5);
    }
}

void MainWindow::saveFile(QString path){
    qDebug() << "path" << path;
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
                 << "index:" << frame->GetDissectResultBase()->GetIndex()
                 << "o-stream:" << tcp->GetStreamIndexPlusOne()
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
    item->setData(Qt::DisplayRole,frame->GetDissectResultBase()->GetIndex());
    this->ui->tableWidget->setItem(row,MainWindow::COL_NO,item);

    if( frame->GetDissectResultBase()->ContainProtocol("tcp") )
        item->setData(Qt::UserRole,qAbs(frame->GetDissectResultBase()->GetAdditionalVal(TCP_STREAM)) - 1);
    else
        item->setData(Qt::UserRole,-1);

    if( this->streamIndex != -1 && item->data(Qt::UserRole).toInt() != this->streamIndex){
        this->ui->tableWidget->hideRow(row);
    }

    //Time
    str.clear();
    str.append(QString::asprintf("%.8lf",frame->GetRelativeTimeSinceFirstPacket()));
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_TIME,item);


    //Source
    str.clear();
    if(frame->GetDissectResultBase()->ContainProtocol("ipv4")){
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
    if(frame->GetDissectResultBase()->ContainProtocol("ipv4")){
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
    str.append(frame->GetDissectResultBase()->GetTopProtocolName());
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_PROTOCOL,item);

    //Length
    item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole,frame->GetCapLen());
    this->ui->tableWidget->setItem(row,MainWindow::COL_LENGTH,item);

    //Summery
    str.clear();
    str.append(frame->GetDissectResultBase()->GetSummery());
    item = new QTableWidgetItem(str);
    this->ui->tableWidget->setItem(row,MainWindow::COL_INFO,item);

    //背景色
    this->addBackgroundToTableRow(frame,row);


    if(this->scrollToBottom)
        this->ui->tableWidget->scrollToBottom();

    /*显示百分比*/
    this->displayProportion->clear();
    this->displayProportion->setText(QString("%1%").arg(this->ui->tableWidget->rowCount()*1.0/this->capturer->GetCount()*100));

    if( !this->haveDate )
        this->haveDate = true;

}


void MainWindow::ergoditTree(QTreeWidgetItem *parent,ProTreeNode *node){
    QTreeWidgetItem *item = nullptr;
    while (node != nullptr) {
        item = new QTreeWidgetItem();
        item->setText(0,node->GetMsg());
        item->setData(0,10001,node->GetStart());
        item->setData(0,10002,node->GetEnd());
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
     qint32 *interfaceId = new qint32(this->capturer->GetCapHandle()->GetDeviceIndex());
     QString *interfaceName = new QString(this->capturer->GetCapHandle()->GetDeviceName());
     qint32 *linklayerType = new qint32(this->capturer->GetCapHandle()->GetLinkType());
     QString *linklayerTypeName = new QString(this->capturer->GetCapHandle()->GetLinkTypeName());
     QList<void*> *reserve = new QList<void*> {this->dissectorOptions,interfaceId,interfaceName,linklayerType,linklayerTypeName};

     ProTreeMaker *maker = new ProTreeMaker(this->capturer->GetCapHandle()->GetLinkType(),this->capturer->GetDissectResultFrameByIndex(index),reserve);
     ProTreeNode *node = maker->GetProTree()->GetHeader();
     this->ui->treeWidget->clear();
     this->ergoditTree(nullptr,node);
}


void MainWindow::addToRawDataPanel(qint64 index){
    const quint8* ptr = this->capturer->GetDissectResultFrameByIndex(index)->GetDissectResultBase()->GetData();
    const quint8* line = nullptr;
    qint32 capLen = this->capturer->GetDissectResultFrameByIndex(index)->GetCapLen();
    QString text;
    qint32 rowCount = 0;
    QTableWidgetItem *item;

    this->ui->rawDataPanel->clearContents();
    this->ui->rawDataPanel->setRowCount(0);
//    for(qint32 i = 0; i < capLen; i++){
//        if(i % 16 == 0){
//            this->ui->rawDataPanel->insertRow(rowCount);
//            this->ui->rawDataPanel->setVerticalHeaderItem(rowCount, new QTableWidgetItem(QString::asprintf("%04x",rowCount)));
//            rowCount++;
//        }
//    }

    rowCount = (capLen % 16 == 0) ? capLen / 16 : capLen / 16 + 1;
    for( qint32 i = 0; i < rowCount; i++ ){
        this->ui->rawDataPanel->insertRow(i);
        this->ui->rawDataPanel->setVerticalHeaderItem(i, new QTableWidgetItem(QString::asprintf("%04x",rowCount)));
    }

    for(qint32 row =0; row < rowCount; row++){
        line = ptr + row * 16;

        for(qint32 col = 0; col < rawDataPanelColCount; col++){
            text.clear();
            item = new QTableWidgetItem(text);
            this->ui->rawDataPanel->setItem(row,col,item);
            if( col == 8 || col == 17 ){
                if( col == 8 )
                    item->setData(Qt::UserRole,row * 16 + col + 1);
                else
                    item->setData(Qt::UserRole, -1);
            }else if( col < 8 ){
                if( row * 16 + col + 1 > capLen ){
                    item->setData(Qt::UserRole,-1);
                    goto finished;
                }
                    //continue;
                item->setData(Qt::UserRole,row * 16 + col);
                text.append(QString::asprintf("%02x",line[col]));
            }else if( col < 17){
                if( row * 16 + col > capLen ){
                    item->setData(Qt::UserRole,-1);
                    goto finished;
                }
                    //continue;
                item->setData(Qt::UserRole,row * 16 + col - 1);
                text.append(QString::asprintf("%02x",line[col - 1]));
            }else{
                if( row * 16 + col - 17 > capLen ){
                    item->setData(Qt::UserRole,-1);
                    goto finished;
                }
                    //break;
                item->setData(Qt::UserRole,row * 16 + col - 18);
                if(isprint(line[col - 18]))
                    text.append(QString::asprintf("%c",line[col - 18]));
                else
                    text.append(QString::asprintf("."));
            }
            finished:
            item->setText(text);
        }
    }

}

void MainWindow::slot_updateSelectedDevice(QListWidgetItem *item){
    this->selectedDevName.clear();
    this->selectedDevName.append(item->text());
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column)
    qint64 index = this->ui->tableWidget->item(row,COL_NO)->text().toULongLong();
    this->addToRawDataPanel(index);
    this->addToTree(index);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    qint32 start = item->data(column,10001).toInt();
    qint32 end = item->data(column,10002).toInt();
    this->clearRawDataPanelBackground(0xffffff);
    qDebug() << "start:" << start;
    qDebug() << "end:" << end;

    if( start == -1 || end == -1 )
        return;

    if( start == -2 || end == -2 ){
        this->clearRawDataPanelBackground(0xff00cc);
        return;
    }

    /*
     *01234567 01234567 0123456701234567
     */
    qint32 startRow = start / 16;
    qint32 endRow = end / 16;
    qint32 no;
    for( qint32 row = startRow; row <= endRow; row++ ){
        for( qint32 col = 0; col < this->ui->rawDataPanel->columnCount(); col++){
            no = this->ui->rawDataPanel->item(row,col)->data(Qt::UserRole).toInt();
            if( no >= start && no <= end )
                this->ui->rawDataPanel->item(row,col)->setBackground(QColor(0xff00cc));
        }
    }
}


/*工具栏相应方法*/
void MainWindow::on_actionStop_triggered()
{
    this->capturer->Stop();
    try {
        this->capturer->GetCapHandle()->SetNonBlock(1);
    } catch (QString e) {
        QMessageBox::critical(this,"Error",e);
    }
    this->ui->actionStart->setEnabled(true);
    this->ui->actionStop->setEnabled(false);
    this->ui->actionRestart->setEnabled(false);
    this->ui->actionDissector_options->setEnabled(true);

    if( this->capturer != nullptr && this->ui->tableWidget->rowCount() == 0){
        StopWithoutAnyPacket stopWithoutAnyPacketDialog;
        stopWithoutAnyPacketDialog.exec();
        this->ui->widget_deviceList->show();
        this->ui->tableWidget->hide();
        this->ui->treeWidget->hide();
        this->ui->rawDataPanel->hide();
    }
}

void MainWindow::on_actionStart_triggered()
{
//    SaveOrCloseFileDialog saveOrCloseFileDialog;
//    saveOrCloseFileDialog.exec();

    if( this->capturer != nullptr && this->ui->widget_deviceList->isHidden()){
        this->saveOrCloseFileDialog->exec();
        if( this->saveOrCloseFileDialog->result() == QDialog::Accepted )
            this->slot_startCapture();
    }else{
        this->slot_startCapture();
    }
//    this->slot_startCapture();
//    this->capturer->quit();
//    if(this->capturer != nullptr)
//        connect(this->capturer,&QThread::finished,this->capturer,&QThread::deleteLater);

//    try {
//        DeviceList::capHandle->ChangeDevice();
//        DeviceList::capHandle->ActivateHandleWithParas();
//        capturer = new Capturer(DeviceList::capHandle,this->dissectorOptions);
//        connect(this->capturer,SIGNAL(onePacketCaptured(DissectResultFrame*)),this,SLOT(addToTable(DissectResultFrame*)));
//        capturer->Start();
//    } catch (QString e) {
//        QMessageBox::critical(this,"Error",e);
//        return;
//    }

//    this->ui->tableWidget->clearContents();
//    this->ui->tableWidget->setRowCount(0);
//    this->ui->treeWidget->clear();
//    this->ui->rawDataPanel->clearContents();
//    this->ui->rawDataPanel->setRowCount(0);
//    this->ui->actionStart->setEnabled(false);
//    this->ui->actionStop->setEnabled(true);
//    this->ui->actionRestart->setEnabled(true);
//    this->ui->actionDissector_options->setEnabled(false);

//    DeviceList::capHandle->ChangeDevice();

//    this->capturer = new Capturer(DeviceList::capHandle,this->dissectorOptions);

//    connect(this->capturer,SIGNAL(onePacketCaptured(DissectResultFrame*)),this,SLOT(addToTable(DissectResultFrame*)));

//    this->capturer->Start();
}

void MainWindow::slot_startCapture(){
    if(this->capturer != nullptr){
        if( this->capturer->isRunning() )
            this->capturer->terminate();
        connect(this->capturer,&QThread::finished,this->capturer,&QThread::deleteLater);
    }

    try {
//        DeviceList::capHandle->ChangeDevice();
//        DeviceList::capHandle->ActivateHandleWithParas();
        CapHandle *capHandle = new CapHandle(this->selectedDevName);
        capHandle->ActivateHandleWithParas();
        if( capHandle->GetLinkType() != 1){
            QMessageBox::critical(this,"Error","Corresponding parser has not been added yet");
            return;
        }
        //capturer = new Capturer(DeviceList::capHandle,this->dissectorOptions);
        capturer = new Capturer(capHandle,this->dissectorOptions);
        connect(this->capturer,SIGNAL(onePacketCaptured(DissectResultFrame*)),this,SLOT(addToTable(DissectResultFrame*)));
        capturer->Start();
    } catch (QString e) {
        QMessageBox::critical(this,"Error",e);
        return;
    }
    if( !this->ui->widget_deviceList->isHidden()){
        this->ui->widget_deviceList->hide();
        this->ui->tableWidget->show();
        this->ui->treeWidget->show();
        this->ui->rawDataPanel->show();
    }

    this->ui->tableWidget->clearContents();
    this->ui->tableWidget->setRowCount(0);
    this->ui->treeWidget->clear();
    this->ui->rawDataPanel->clearContents();
    this->ui->rawDataPanel->setRowCount(0);
    this->ui->actionStart->setEnabled(false);
    this->ui->actionStop->setEnabled(true);
    this->ui->actionRestart->setEnabled(true);
    this->ui->actionDissector_options->setEnabled(false);
}

void MainWindow::slot_saveFileBeforeCapture(QString path){
    this->saveFile(path);
    this->slot_startCapture();
}

void MainWindow::on_actionRestart_triggered()
{
    this->on_actionStop_triggered();
    this->on_actionStart_triggered();
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

void MainWindow::on_pushButton_filter_clicked()
{
    if( this->ui->lineEdit->text().isEmpty() ){
        this->streamIndex = -1;
        for( qint64 index = 0; index < this->ui->tableWidget->rowCount() - 1; index++ ){
            this->ui->tableWidget->showRow(index);
        }
    }else{
        this->streamIndex = this->ui->lineEdit->text().toInt();
        for( qint64 index = 0; index < this->ui->tableWidget->rowCount() - 1; index++ ){
            if( this->ui->tableWidget->item(index,COL_NO)->data(Qt::UserRole).toInt() == this->streamIndex ){
                this->ui->tableWidget->showRow(index);
            }else{
                this->ui->tableWidget->hideRow(index);
            }
        }
    }
}



/*菜单栏*/
void MainWindow::on_actionDissector_options_triggered()
{
    DissectorOptions dp;
    dp.exec();
}


/*Device List*/
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    this->slot_updateSelectedDevice(item);
    this->slot_startCapture();
}
