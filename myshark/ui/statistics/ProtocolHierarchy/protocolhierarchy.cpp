#include "protocolhierarchy.h"
#include "ui_protocolhierarchy.h"

ProtocolHierarchy::ProtocolHierarchy(Capturer *capturer, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProtocolHierarchy),
    capturer(capturer)
{
    ui->setupUi(this);

    this->allPackets = capturer->GetCount();
    this->allTime = capturer->GetDissectResultFrameByIndex(allPackets - 1)->GetRelativeTimeSinceFirstPacket();
    for( qint32 index = 0; index < allPackets; index++ )
        this->allBytes += capturer->GetDissectResultFrameByIndex(index)->GetCapLen();

    this->ui->treeWidget->setColumnCount(headers.length());
    this->ui->treeWidget->setHeaderLabels(headers);

    this->tree = new HierarchyTree();
    for( qint32 index = 0; index < capturer->GetCount(); index++ )
        this->tree->UpdateTree(capturer->GetDissectResultFrameByIndex(index));

    this->addToTree();
}

ProtocolHierarchy::~ProtocolHierarchy()
{
    delete ui;
}

void ProtocolHierarchy::addToTree(){
    HierarchyNode *root = this->tree->GetHeader();

    QTreeWidgetItem *item = new QTreeWidgetItem();
    this->addToTreeItem(item,root);
    this->ui->treeWidget->addTopLevelItem(item);

    this->addToTreeLoop(item,root);
}

void ProtocolHierarchy::addToTreeLoop(QTreeWidgetItem *parentItem,HierarchyNode *parentNode){
    HierarchyNode *node = nullptr;
    QTreeWidgetItem *item = nullptr;
    if( parentNode->GetChildCount() > 0 ){
        for( qint32 index = 0; index < parentNode->GetChildCount(); index++ ){
            node = parentNode->GetChildByIndex(index);
            item = new QTreeWidgetItem();
            this->addToTreeItem(item,node);
            parentItem->addChild(item);
            this->addToTreeLoop(item,node);
        }
    }
}

void ProtocolHierarchy::addToTreeItem(QTreeWidgetItem *item, HierarchyNode *node){
    item->setText(PROTOCOL,node->GetName());
    item->setText(PACKETS,QString("%1").arg(node->GetPackets()));
    if( this->allPackets != 0 )
        item->setText(PERCENTPACKETS,QString::asprintf("%.1f",node->GetPackets() * 1.0 / this->allPackets * 100) + "%");
    else
        item->setText(PERCENTPACKETS,QString("%0"));
    item->setText(BYTES,QString("%1").arg(node->GetBytes()));
    if( this->allBytes != 0 )
        item->setText(PERCENTBYTES,QString::asprintf("%.1f",node->GetBytes() * 1.0 / this->allBytes * 100) + "%");
    else
        item->setText(PERCENTBYTES,QString("%0"));
    if( this->allTime > 0 )
        item->setText(BITSPERSECOND,QString::asprintf("%.1f",node->GetBytes() * 1.0 / this->allTime));
    else
        item->setText(BITSPERSECOND,QString("0"));
}
