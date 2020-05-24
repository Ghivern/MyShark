#include "hierarchytree.h"

HierarchyTree::HierarchyTree()
{
    this->header = new HierarchyNode("frame");
}

HierarchyNode* HierarchyTree::GetHeader(){
    return this->header;
}

void HierarchyTree::UpdateTree(DissectResultFrame *frame){
    HierarchyNode *node = this->header;
    qint32 protocolNum = frame->GetDissectResultBase()->GetProtocolListLength();
    QString name;
    node->UpdateData(frame->GetCapLen());
    //QString protocolName;
    //qDebug() << frame->GetDissectResultBase()->GetProtocolNameByIndex(0);
    for( qint32 index = 0; index < protocolNum; index++ ){
        name.clear();
        name.append(frame->GetDissectResultBase()->GetProtocolNameByIndex(index));
        //protocolName.append(name + "->");

        if( !node->ContainsChild(name) ){
            HierarchyNode *tempNode = new HierarchyNode(name);
            node->AppendChild(tempNode);
            node = tempNode;
        }else{
            node = node->GetChildByName(name);
        }
        //qDebug() << frame->GetDissectResultBase()->GetProtocolStart(name);
        //node->UpdateData(frame->GetCapLen() - frame->GetDissectResultBase()->GetProtocolStart(name));
        //node->UpdateData(frame->GetCapLen());
        node->UpdateData(frame->GetDissectResultBase()->GetProtocolLength(name));
    }
    //qDebug() << protocolName;
}


