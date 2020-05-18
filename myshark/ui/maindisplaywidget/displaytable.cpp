#include "displaytable.h"



DisplayTable::DisplayTable(QWidget *parent) : QTableWidget(parent)
{


    QStringList headers = {"No.","Time","Source","Destination","Protocol","Length","Info."};
    this->setHorizontalHeaderLabels(headers);
    this->verticalHeader()->setVisible(false);

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->scrollToLastRow = false;

    this->displayFilter = new DisplayFilter;

    connect(this,SIGNAL(cellClicked(int,int)),this,SLOT(slot_rowSelected(qint32,qint32)));
}

DisplayTable::~DisplayTable()
{

}

void DisplayTable::SetScrollToBottom(bool scrollToLastRow){
    this->scrollToLastRow = scrollToLastRow;
    if( scrollToLastRow )
        this->scrollToBottom();
}

bool DisplayTable::SetFilter(QString filterStr){
    if( this->displayFilter->SetFilter(filterStr) ){
        qint32 rowCount = this->rowCount();
        for(qint32 row = 0; row < rowCount ; row++){
//            if( !this->displayFilter->Filte( (DissectResultFrame*)this
//                                            ->item(row,COL_NO)->data(Qt::UserRole).value<void*>()) ){
//                this->hideRow(row);
//            }else{
//                this->showRow(row);
//            }
            if( !this->displayFilter->FilteWithoutLogicOperator((DissectResultFrame*)this
                                            ->item(row,COL_NO)->data(Qt::UserRole).value<void*>()) ){
                this->hideRow(row);
            }else{
                this->showRow(row);
            }
        }
        emit showItemsCountChange(this->displayFilter->GetDisplayedCount(),this->rowCount());
        return true;
    }
    return false;
}

DisplayFilter* DisplayTable::GetDisplayFilter(){
    return this->displayFilter;
}

qint64 DisplayTable::GetDisplayedPacketCount(){
    return this->displayFilter->GetDisplayedCount();
}

void DisplayTable::Clear(){
    this->clearContents();
    this->setRowCount(0);
    this->displayFilter->SetFilter("");
}

void DisplayTable::SetFontPointSize(qint32 pointSize){
    QFont font = this->font();
    if( pointSize == 0 ){
        font.setPointSize(font.pointSize() + 1);
    }else if( pointSize == - 1){
        font.setPointSize(font.pointSize() - 1);
    }else{
        font.setPointSize(pointSize);
    }
    QFontMetrics fontMetrics(font);
    this->verticalHeader()->setDefaultSectionSize(fontMetrics.height());
    this->setFont(font);
}

void DisplayTable::ResizeTableToFitContent(){
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void DisplayTable::slot_addToTable(DissectResultFrame *frame){
    qint32 row = this->addToTable(frame);
    this->addStatusColor(frame,row);
}

qint32 DisplayTable::addToTable(DissectResultFrame *frame){
    QTableWidgetItem *item;
    QString str;
    qint32 row = this->rowCount();

    this->insertRow(row);

//    if( !this->displayFilter->Filte(frame) )
//        this->hideRow(row);
    if( !this->displayFilter->FilteWithoutLogicOperator(frame) )
        this->hideRow(row);

    emit showItemsCountChange(this->displayFilter->GetDisplayedCount(),this->rowCount());

    //No.
    item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole,frame->GetDissectResultBase()->GetIndex());
    this->setItem(row,COL_NO,item);

    item->setData(Qt::UserRole,QVariant::fromValue((void*)frame));


    //Time
    str.clear();
    str.append(QString::asprintf("%.8lf",frame->GetRelativeTimeSinceFirstPacket()));
    item = new QTableWidgetItem(str);
    this->setItem(row,COL_TIME,item);


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
    this->setItem(row,COL_SOURCE,item);

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
    this->setItem(row,COL_DESTINATION,item);

    //Protocol
    str.clear();
    str.append(frame->GetDissectResultBase()->GetTopProtocolName());
    item = new QTableWidgetItem(str);
    this->setItem(row,COL_PROTOCOL,item);

    //Length
    item = new QTableWidgetItem();
    item->setData(Qt::DisplayRole,frame->GetCapLen());
    this->setItem(row,COL_LENGTH,item);

    //Summery
    str.clear();
    str.append(frame->GetDissectResultBase()->GetSummery());
    item = new QTableWidgetItem(str);
    this->setItem(row,COL_INFO,item);

    if(this->scrollToLastRow)
        this->scrollToBottom();

    return row;
}

void DisplayTable::addStatusColor(DissectResultFrame *frame,qint32 row){
    if( frame->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR) ){
        TcpInfo *tcpInfo = (TcpInfo*)frame->GetDissectResultBase()->GetAdditionalPtr(TCP_INFO_PTR);
        if( tcpInfo->status != 0 || tcpInfo->badChecksum){
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

void DisplayTable::setTableWidgetColor(qint32 row,quint32 background, quint32 textColor){
    for(qint32 col = 0; col < COL_COUNT ; col++){
        if( this->item(row,col) ){
            this->item(row,col)->setBackground(QColor(background));
            this->item(row,col)->setForeground(QColor(textColor));
        }
    }
}

void DisplayTable::setTableWidgetColor(qint32 row, quint32 background){
    for(qint32 col = 0; col < COL_COUNT; col++){
        if( this->item(row,col) ){
            this->item(row,col)->setBackground(QColor(background));
        }
    }
}

void DisplayTable::slot_rowSelected(qint32 row,qint32 column){
    Q_UNUSED(column)
    emit rowSelected((DissectResultFrame*)this->item(row,COL_NO)->data(Qt::UserRole).value<void*>());
}
