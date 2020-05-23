#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../dissector/dissres/dissectresutltframe.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //this->readyToQuit = false;

//    this->streamIndex = -1;
     this->capturer = new Capturer();
    //this->packetsNeedToBeSavedBeforeStart = false;

    //this->displayedRowCount = 0;

    //this->captureFileProperties = nullptr;

    //this->fromFile = false;
    //this->filePath = "";

    //this->tempFile = nullptr;
    this->showDisplayProportion = new DisplayProportion;

    this->setupUi();

    this->setupSignal();


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent(QCloseEvent *event){
    if( this->capturer->GetCapHandle()->FromFile() ){
        event->accept();
    }else{
        if( this->capturer->isRunning() ){
            if( this->capturer->slot_stopThread() ){
                event->accept();
            }else{
                goto capture_stopped;
            }
        }else{
            capture_stopped:
            if( this->capturer->slot_clearThread(3) ){
                event->accept();
            }else{
                this->setActionsStatus(STATUS_STOP);
                event->ignore();
            }
        }
    }
}

void MainWindow::setupUi(){
    this->setActionsStatus(STATUS_NONE);

    /*MainWindow*/
    this->resize(1050,650);
    this->setWindowTitle("MyShark");

    this->on_actionDefaultTextSize_triggered();

    /*Display filter*/
    this->ui->lineEdit->setPlaceholderText("Display filter");

    /*StatuBar*/
    this->ui->statusbar->addWidget(this->showDisplayProportion);
    this->showDisplayProportion->SetDefaultInfo();

}

void MainWindow::setupSignal(){
    /*安装事件过滤器*/
    this->ui->tableWidget->verticalScrollBar()->installEventFilter(this);

    connect(this->capturer,SIGNAL(started()),this,SLOT(slot_changeWindowTitle()));


    connect(this->capturer,SIGNAL(onePacketCaptured(DissectResultFrame*)),this->ui->tableWidget,SLOT(slot_addToTable(DissectResultFrame*)));
    connect(this->ui->tableWidget,SIGNAL(rowSelected(DissectResultFrame*)),this->ui->treeWidget,SLOT(slot_addToTree(DissectResultFrame*)));
    connect(this->ui->tableWidget,SIGNAL(rowSelected(DissectResultFrame*)),this->ui->rawDataPanel,SLOT(slot_addToRawDataPanel(DissectResultFrame*)));
    connect(this->ui->treeWidget,SIGNAL(oneItemSelected(qint32,qint32)),this->ui->rawDataPanel,SLOT(slot_hightLigth(qint32,qint32)));

    /*改变Window的Title*/
    connect(this->ui->listWidget,SIGNAL(selectedInterfaceChanged(QString,bool)),this,SLOT(slot_start(QString,bool)));

    /*Status Bar显示百分比*/
    connect(this->ui->tableWidget,SIGNAL(showItemsCountChange(qint32,qint32)),this->showDisplayProportion,SLOT(slot_setData(qint32,qint32)));

}

void MainWindow::setActionsStatus(enum WINDOW_STATUS status){
    switch (status)
    {
    case STATUS_CAPTURING:
    {
        this->ui->actionStart->setEnabled(false);
        this->ui->actionStop->setEnabled(true);
        this->ui->actionRestart->setEnabled(true);
        this->ui->actionOpen->setEnabled(false);
        this->ui->actionClose->setEnabled(false);
        this->ui->actionSave->setEnabled(false);
        this->ui->actionSave_As->setEnabled(false);

        this->ui->actionDissector_options->setEnabled(false);

        this->ui->actionCapture_file_properitys->setEnabled(true);
        this->ui->actionConversations->setEnabled(true);
        this->ui->actionactionEndpoints->setEnabled(true);
        this->ui->actionPacketLengths->setEnabled(true);

        this->showDisplayProportion->SetDefaultInfo();
        break;
    }
    case STATUS_STOP:
    {
        this->ui->actionStart->setEnabled(true);
        this->ui->actionStop->setEnabled(false);
        this->ui->actionRestart->setEnabled(false);
        this->ui->actionOpen->setEnabled(true);
        this->ui->actionClose->setEnabled(true);
        this->ui->actionSave->setEnabled(true);
        this->ui->actionSave_As->setEnabled(true);

        this->ui->actionDissector_options->setEnabled(false);

        this->ui->actionCapture_file_properitys->setEnabled(true);
        this->ui->actionConversations->setEnabled(true);
        this->ui->actionactionEndpoints->setEnabled(true);
        this->ui->actionPacketLengths->setEnabled(true);
        break;
    }
    case STATUS_FILE:
    {
        this->ui->actionStart->setEnabled(true);
        this->ui->actionStop->setEnabled(false);
        this->ui->actionRestart->setEnabled(false);
        this->ui->actionOpen->setEnabled(true);
        this->ui->actionClose->setEnabled(true);
        this->ui->actionSave->setEnabled(false);
        this->ui->actionSave_As->setEnabled(true);

        this->ui->actionDissector_options->setEnabled(false);

        this->ui->actionCapture_file_properitys->setEnabled(true);
        this->ui->actionConversations->setEnabled(true);
        this->ui->actionactionEndpoints->setEnabled(true);
        this->ui->actionPacketLengths->setEnabled(true);

        this->showDisplayProportion->SetDefaultInfo();
        break;
    }
    default:
    {
        this->ui->actionStart->setEnabled(true);
        this->ui->actionStop->setEnabled(false);
        this->ui->actionRestart->setEnabled(false);
        this->ui->actionOpen->setEnabled(true);
        this->ui->actionClose->setEnabled(false);
        this->ui->actionSave->setEnabled(false);
        this->ui->actionSave_As->setEnabled(false);

        this->ui->actionDissector_options->setEnabled(true);

        this->ui->actionCapture_file_properitys->setEnabled(false);
        this->ui->actionConversations->setEnabled(false);
        this->ui->actionactionEndpoints->setEnabled(false);
        this->ui->actionPacketLengths->setEnabled(false);

        this->showDisplayProportion->SetDefaultInfo();
    }
    }

    if( status == STATUS_NONE ){
        this->ui->splitter->hide();
        this->ui->widget_deviceList->show();
        this->ui->lineEdit->setEnabled(false);
        this->ui->pushButton_filter->setEnabled(false);
    }else{
        this->ui->splitter->show();
        this->ui->widget_deviceList->hide();
        this->ui->lineEdit->setEnabled(true);
        this->ui->pushButton_filter->setEnabled(false);
    }
}

/*事件过滤器*/
bool MainWindow::eventFilter(QObject *target, QEvent *event){
    if (target == this->ui->tableWidget->verticalScrollBar()){
        if(event->type() == QEvent::Wheel)
        {
            QWheelEvent *wheelEvent = (QWheelEvent*)event;
            if (wheelEvent->delta() > 0) {
                this->ui->tableWidget->SetScrollToBottom(false);
                this->ui->actionScrollToLastLine->setChecked(false);
            }
        }
    }
    return QMainWindow::eventFilter(target, event);
}

/*控制启止*/
void MainWindow::slot_start(QString interfaceOrFile,bool fromFile){
    if( this->capturer->slot_startThread(interfaceOrFile,fromFile) ){
        this->ui->widget_deviceList->hide();
        this->ui->splitter->show();
        this->setActionsStatus(STATUS_CAPTURING);
    }
}

void MainWindow::on_actionStart_triggered(){
    if( this->capturer->slot_startThread(this->ui->listWidget->GetSelectedInterface(),false) ){
        this->ui->tableWidget->Clear();
        this->ui->treeWidget->Clear();
        this->ui->rawDataPanel->Clear();
        this->setActionsStatus(STATUS_CAPTURING);
    }
}

void MainWindow::on_actionStop_triggered(){
    if( this->capturer->slot_stopThread() ){  /*未抓到任何数据*/
        this->setActionsStatus(STATUS_NONE);
        return;
    }
    this->setActionsStatus(STATUS_STOP);
}

void MainWindow::on_actionRestart_triggered(){
    this->on_actionStop_triggered();
    this->on_actionStart_triggered();
}

void MainWindow::on_actionOpen_triggered(){
    QString path = QFileDialog::getOpenFileName(nullptr
                                                ,"Save file"
                                                ,QDir::currentPath()
                                                ,"all(*.*);;pcap(*.pcap);;pcapng(*.pcapng)");
    if( !path.isEmpty() ){
        if( this->capturer->slot_startThread(path,true) ){
            this->ui->tableWidget->Clear();
            this->ui->treeWidget->Clear();
            this->ui->rawDataPanel->Clear();
            this->setActionsStatus(STATUS_FILE);
        }
    }
}

void MainWindow::on_actionSave_triggered(){
    if( this->capturer->GetCapHandle() != nullptr ){
        if( this->capturer->slot_saveTempFile() ){
            this->ui->actionSave->setEnabled(false);
        }
    }
}

void MainWindow::on_actionSave_As_triggered(){
    if( this->capturer->GetCapHandle() != nullptr ){
        if( this->capturer->slot_saveTempFile() ){
            this->ui->actionSave->setEnabled(false);
        }
    }
}

void MainWindow::on_actionClose_triggered(){
    if( this->capturer->GetCapHandle() != nullptr ){
        if( this->capturer->slot_clearThread(2) ){
            this->setActionsStatus(STATUS_NONE);
            this->ui->tableWidget->Clear();
            this->ui->treeWidget->Clear();
            this->ui->rawDataPanel->Clear();
        }
    }
}

void MainWindow::on_actionQuit_triggered(){
    emit close();
}

/*显示过滤器*/
void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if( this->ui->lineEdit->text().isEmpty() ){
        this->ui->lineEdit->setStyleSheet("background-color:gray");
        this->ui->pushButton_filter->setEnabled(true);
    }else{
        if( this->ui->tableWidget->GetDisplayFilter()->FilterIsValied(arg1) ){
            this->ui->lineEdit->setStyleSheet("background-color:green");
            this->ui->pushButton_filter->setEnabled(true);
        }else{
           this->ui->lineEdit->setStyleSheet("background-color:red");
           this->ui->pushButton_filter->setEnabled(false);
        }
    }
}

void MainWindow::on_pushButton_filter_clicked()
{
    QString filterStr = this->ui->lineEdit->text();
    if( this->ui->tableWidget->GetDisplayFilter()->FilterIsValied(filterStr)){
        this->ui->tableWidget->SetFilter(filterStr);
        this->ui->pushButton_filter->setEnabled(false);
    }
}

/*改变Window的Title*/
void MainWindow::slot_changeWindowTitle(){
    QString title = "";
    if( this->capturer->GetCapHandle() != nullptr )
        title.append(this->capturer->GetCapHandle()->FromFile() ?
                                  this->capturer->GetCapHandle()->GetFileName()
                                  :this->capturer->GetCapHandle()->GetDeviceName()
                                  );
    else
        title.append("MyShark");
    this->setWindowTitle(title);
}



/*控制显示效果*/
void MainWindow::on_actionEnlargeTextSize_triggered()
{
    this->ui->tableWidget->SetFontPointSize();
    this->ui->treeWidget->SetFontPointSize();
    this->ui->rawDataPanel->SetFontPointSize();
}

void MainWindow::on_actionShrinkTextSize_triggered()
{
    this->ui->tableWidget->SetFontPointSize(-1);
    this->ui->treeWidget->SetFontPointSize(-1);
    this->ui->rawDataPanel->SetFontPointSize(-1);
}

void MainWindow::on_actionDefaultTextSize_triggered()
{
    this->ui->tableWidget->SetFontPointSize(this->defaultTextSize);
    this->ui->treeWidget->SetFontPointSize(this->defaultTextSize);
    this->ui->rawDataPanel->SetFontPointSize(this->defaultTextSize);
}

void MainWindow::on_actionScrollToLastLine_triggered(bool checked)
{
    this->ui->tableWidget->SetScrollToBottom(checked);
}

void MainWindow::on_actionResizeTableWidgetTOFitContents_triggered()
{
    this->ui->tableWidget->ResizeTableToFitContent();
}


/*解析器选项面板*/
void MainWindow::on_actionDissector_options_triggered()
{
    DissectorOptions dp;
    dp.exec();
}


/*统计结果显示面板*/
void MainWindow::on_actionCapture_file_properitys_triggered()
{
    CaptureFileProperties *cfp = new CaptureFileProperties;
    QString path = "";
    if( this->capturer->GetCapHandle() != nullptr ){
        path.append(this->capturer->GetFilePath());
    }else{
        return;
    }

    cfp->SetFile(path,this->capturer->GetCapHandle()->GetLinkTypeName());


    qint64 firstPacketTime = 0;
    qint64 lastPacketTime = 0;
    if( this->capturer->GetCount() > 0 ){
        firstPacketTime = this->capturer->GetDissectResultFrameByIndex(0)->GetCaptureTimeSec();
        lastPacketTime = this->capturer->GetDissectResultFrameByIndex(this->capturer->GetCount() - 1)->GetCaptureTimeSec();
    }
    cfp->SetTime(firstPacketTime,lastPacketTime);

    cfp->SetCapture();

    cfp->SetInterface(this->capturer->GetCapHandle()->GetDeviceName()
                       ,0,"none",this->capturer->GetCapHandle()->GetLinkTypeName(),0
                      );

    cfp->SetText();
    cfp->show();
}

void MainWindow::on_actionConversations_triggered()
{
    Conversations *conversations = new Conversations(this->capturer);
    conversations->show();
    //qDebug() <<"stream1 包量:" << tcp_ip_protocol_family::DissectResultLinkLayer::GetStreamRecorder().GetPacketsIndexListByStream(1).length();
}

void MainWindow::on_actionactionEndpoints_triggered()
{
    Endpoints *endpoints = new Endpoints(this->capturer);
    //qDebug() << this->capturer->GetCount();
    endpoints->show();
}

void MainWindow::on_actionPacketLengths_triggered()
{
    PacketLength *packetLength = new PacketLength(this->capturer);
    packetLength->show();
}




///*用于测试的，暂时性的*/
//void MainWindow::PrintProTree(ProTreeNode *proTreeNode, qint32 level){
//    while (proTreeNode != NULL){
//        QString str;
//        qint32 index = 0;
//        while (index < level) {
//            if(level == 1)
//                str += "@";
//            else
//                str += "=";
//            index++;
//        }
//        qDebug() << str << proTreeNode->GetMsg() << proTreeNode->GetStart() << "~" << proTreeNode->GetEnd();
//        if( proTreeNode->GetNextLevel() != NULL)
//            PrintProTree(proTreeNode->GetNextLevel(),level + 5);
//        proTreeNode = proTreeNode->GetNext();
//    }
//}


///*用于测试的，暂时性的*/
//void MainWindow::Print(qint64 index){
//    DissectResultFrame *frame = this->capturer->GetDissectResultFrameByIndex(index);
//    if( *(quint16*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypePtr() == tcp_ip_protocol_family::DissectResultLinkLayer::LINKLAYER_TYPE_IPV4){
//        tcp_ip_protocol_family::DissectResultIpv4 *ipv4 = (tcp_ip_protocol_family::DissectResultIpv4*)frame->GetTcpIpProtocolFamilyBaseLayer()->GetNextLayer();

//        if(ipv4->GetType() == tcp_ip_protocol_family::DissectResultIpv4::NETWORKLAYER_IPV4_TYPE_TCP){
//            tcp_ip_protocol_family::DissectResultTcp *tcp = (tcp_ip_protocol_family::DissectResultTcp*)ipv4->GetNextLayer();
//            qDebug() << "Tcp->"
//                 << "index:" << frame->GetDissectResultBase()->GetIndex()
//                 << "o-stream:" << tcp->GetStreamIndexPlusOne()
//                 << "srcPort:" << tcp->GetSourcePort()
//                 << "dstPort:" << tcp->GetDestinationPort()
//                 << (tcp->SYN()?"SYN":"")
//                 << "seq:" << tcp->GetSeq()
//                 << "r-seq:" << tcp->GetRelativeSeq()
//                 << "len:" << tcp->GetPayloadLen()
//                 //<< "ack:" << tcp->GetAck()
//                 //<< "r-ack:" << tcp->GetRelativeAck()
//                 << "pre:" <<((tcp->GetPrevious() == -1)?"无":QString("%1").arg(tcp->GetPrevious()))
//                 << "status:" << tcp->GetSegmentStatusStr()
//                 << "max-segment:" << tcp->GetOptionMaximumSegmentSize()
//                    ;
//        }else{
//            //qDebug() << "未处理的运输层协议，协议号为" << ipv4->GetType();
//        }
//    }else{
//        //qDebug() << "未处理的网络层协议，协议号为" << frame->GetTcpIpProtocolFamilyBaseLayer()->GetTypeStr();
//    }
//}










