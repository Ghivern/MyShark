#include "packetlength.h"
#include "ui_packetlength.h"

PacketLength::PacketLength(Capturer *capturer,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PacketLength)
    ,capturer(capturer)
{
    ui->setupUi(this);

    ui->treeWidget->setColumnCount(headers.length());
    ui->treeWidget->setHeaderLabels(headers);

    this->addToTree();
}

PacketLength::~PacketLength()
{
    delete ui;
}

void PacketLength::statics(staticsResult *result,qint32 start,qint32 end){
    qint32 capLen = 0;
    result->count = 0;
    result->maxVal = 0;
    result->minVal = 0;
    result->bytes = 0;
    result->topicItem.clear();

    if( end == - 1 ){
        end = 0x7fffffff;   //end == -1 标识无限，将end赋值为qint32可以表示的最大值
        if( start == 0 )
            result->topicItem.append("Packets");
        else
            result->topicItem.append( QString("%1 and greater").arg(start) );
    }else{
        result->topicItem.append( QString("%1 - %2").arg(start).arg(end) );
    }

    for( qint32 index = 0; index < this->capturer->GetCount(); index++ ){
        capLen = this->capturer->GetDissectResultFrameByIndex(index)->GetCapLen();
        if( capLen >= start && capLen < end ){
            result->count++;
            result->bytes += capLen;
            result->maxVal = result->maxVal > capLen ? result->maxVal : capLen;
            if( result->count == 1 )
                result->minVal = capLen;
            else
                result->minVal = result->minVal > capLen ? capLen : result->minVal;
        }
    }
}

void PacketLength::addToTreeWidgetItem(staticsResult_t *result, QTreeWidgetItem *item,qint32 totalCount,float totalTime){
    item->setText(TOPICITEM,result->topicItem);
    item->setText(COUNT,QString::asprintf("%d",result->count));
    item->setText(AVERAGE,result->count == 0 ? QString("-") : QString::asprintf("%.2f",result->bytes * 1.0 / result->count));
    item->setText(MINVAL,result->count == 0 ? QString("-") : QString("%1").arg(result->minVal));
    item->setText(MAXVAL,result->count == 0 ? QString("-") : QString("%2").arg(result->maxVal));
    item->setText(RATE,result->count == 0 ? QString("-") : QString::asprintf("%.2f",result->count * 1.0 / totalTime));
    item->setText(PERCENT,result->count == 0 ? QString("-") : QString::asprintf("%.2f",result->count * 1.0 / totalCount * 100) + "%");
}

void PacketLength::addToTree(){
    qint32 packetsTotalCount = this->capturer->GetCount();
    float allTime = 0;
    staticsResult_t result;
    QTreeWidgetItem *item = nullptr;
    QTreeWidgetItem *top = nullptr;

    if( this->capturer->GetCount() > 0)
        allTime = this->capturer->GetDissectResultFrameByIndex(packetsTotalCount - 1)->GetRelativeTimeSinceFirstPacket();

    this->statics(&result);
    top = new QTreeWidgetItem();
    this->addToTreeWidgetItem(&result,top,packetsTotalCount,allTime);
    this->ui->treeWidget->addTopLevelItem(top);

    qint32 index;
    for( index = 0; index <= 2560;  ){

        this->statics(&result,index,index == 0 ? 19 : 2 * index - 1);
        item = new QTreeWidgetItem();
        this->addToTreeWidgetItem(&result,item,packetsTotalCount,allTime);
        top->addChild(item);
        if( index == 0 )
            index += 20;
        else
            index *= 2;
    }

    this->statics(&result,index);
    item = new QTreeWidgetItem();
    this->addToTreeWidgetItem(&result,item,packetsTotalCount,allTime);
    top->addChild(item);
}
