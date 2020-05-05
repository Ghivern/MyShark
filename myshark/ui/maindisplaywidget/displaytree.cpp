#include "displaytree.h"

DisplayTree::DisplayTree(QWidget *parent)
    : QTreeWidget(parent)
{
    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slot_itemSelected(QTreeWidgetItem*,qint32)));
}

void DisplayTree::Clear(){
    this->clear();
}

void DisplayTree::SetFontPointSize(qint32 pointSize){
    QFont font = this->font();
    if( pointSize == 0 ){
        font.setPointSize(font.pointSize() + 1);
    }else if( pointSize == - 1){
        font.setPointSize(font.pointSize() - 1);
    }else{
        font.setPointSize(pointSize);
    }
    this->setFont(font);
}

void DisplayTree::slot_addToTree(DissectResultFrame *frame){
    this->addToTree(frame);
}

void DisplayTree::ergoditTree(QTreeWidgetItem *parent,ProTreeNode *node){
    QTreeWidgetItem *item = nullptr;
    while (node != nullptr) {
        item = new QTreeWidgetItem();
        item->setText(0,node->GetMsg());
        item->setData(0,10001,node->GetStart());
        item->setData(0,10002,node->GetEnd());
        if(parent != nullptr)
            parent->addChild(item);
        else
            this->addTopLevelItem(item);
        if(node->GetNextLevel() != nullptr)
            this->ergoditTree(item,node->GetNextLevel());
        node = node->GetNext();
    }
}

void DisplayTree::addToTree(DissectResultFrame *frame){
     ProTreeMaker *maker = new ProTreeMaker(1,frame,nullptr);
     ProTreeNode *node = maker->GetProTree()->GetHeader();
     this->clear();
     this->ergoditTree(nullptr,node);
}

void DisplayTree::slot_itemSelected(QTreeWidgetItem *item,qint32 column){
    emit oneItemSelected(item->data(column,10001).toInt(),item->data(column,10002).toInt());
}
