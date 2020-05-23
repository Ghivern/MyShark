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
    for( qint32 index = 1; index < protocolNum; index++ ){
        name.clear();
        name.append(frame->GetDissectResultBase()->GetProtocolNameByIndex(index));
        if( !node->ContainsChild(name) ){
            node->AppendChild(new HierarchyNode(name));
        }
        node = node->GetChildByName(name);

        node->UpdateData(frame->GetCapLen());
    }
}


