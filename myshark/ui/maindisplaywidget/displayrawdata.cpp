#include "displayrawdata.h"
#include <QHeaderView>

DisplayRawData::DisplayRawData(QWidget *parent)
    : QTableWidget(parent)
{
//    this->ui->rawDataPanel->setFont(font);
//    this->ui->treeWidget->setFont(font);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->verticalHeader()->setVisible(true);
    this->horizontalHeader()->hide();
    this->setGridStyle(Qt::PenStyle::NoPen);

    this->verticalHeader()->setMinimumSectionSize(0);
//    this->verticalHeader()->setDefaultSectionSize(fontMetrics.height());

    this->setColumnCount(rawDataPanelColCount);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void DisplayRawData::Clear(){
    this->clearContents();
    this->setRowCount(0);
}

void DisplayRawData::SetFontPointSize(qint32 pointSize){
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

void DisplayRawData::slot_addToRawDataPanel(DissectResultFrame *frame){
    this->addToRawDataPanel(frame);
}

void DisplayRawData::slot_hightLigth(qint32 start,qint32 end){
    this->hightLight(start,end);
}

void DisplayRawData::addToRawDataPanel(DissectResultFrame *frame){
    const quint8* ptr = frame->GetDissectResultBase()->GetData();
    const quint8* line = nullptr;
    qint32 capLen = frame->GetCapLen();
    QString text;
    qint32 rowCount = 0;
    QTableWidgetItem *item;

    this->clearContents();
    this->setRowCount(0);

    rowCount = (capLen % 16 == 0) ? capLen / 16 : capLen / 16 + 1;
    for( qint32 i = 0; i < rowCount; i++ ){
        this->insertRow(i);
        this->setVerticalHeaderItem(i, new QTableWidgetItem(QString::asprintf("%04x",rowCount)));
    }

    for(qint32 row =0; row < rowCount; row++){
        line = ptr + row * 16;

        for(qint32 col = 0; col < this->rawDataPanelColCount; col++){
            text.clear();
            item = new QTableWidgetItem(text);
            this->setItem(row,col,item);
            if( col == 8 || col == 17 ){
                if( col == 8 )
                    item->setData(Qt::UserRole,row * 16 + col - 0.5);
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

void DisplayRawData::clearRawDataPanelBackground(quint32 background){
    qint32 row,col;
    for( row = 0; row < this->rowCount() - 1; row++ ){
        for( col = 0; col < this->columnCount(); col++ ){
            if( col != 17 )
                this->item(row,col)->setData(Qt::BackgroundRole,QColor(background));
        }
    }

    for( col = 0; col < this->columnCount(); col++ ){
        if( !this->item(row,col)->text().isEmpty() ){
            this->item(row,col)->setData(Qt::BackgroundRole,QColor(background));
        }
    }

    if( !this->item(row,9)->text().isEmpty() ){
        this->item(row,8)->setData(Qt::BackgroundRole,QColor(background));
    }
}


void DisplayRawData::hightLight(qint32 start, qint32 end)
{
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
    float no;
    for( qint32 row = startRow; row <= endRow; row++ ){
        for( qint32 col = 0; col < this->columnCount(); col++){
            no = this->item(row,col)->data(Qt::UserRole).toFloat();
            if( no >= start && no <= end )
                this->item(row,col)->setBackground(QColor(0xff00cc));
        }
    }
}
