#include "hierarchynode.h"

HierarchyNode::HierarchyNode(QString name)
{
    this->name.append(name);
    this->packets = 0;
    this->bytes = 0;
}

qint32 HierarchyNode::GetChildCount(){
    return this->children.length();
}

HierarchyNode* HierarchyNode::GetChildByIndex(qint32 index){
    return this->children[index];
}

HierarchyNode* HierarchyNode::GetChildByName(QString name){
    for( qint32 index = 0; index < this->GetChildCount(); index++ ){
        if( this->GetChildByIndex(index)->Fit(name) )
            return this->children[index];
    }
    return nullptr;
}

void HierarchyNode::AppendChild(HierarchyNode *newChild){
    this->children.append(newChild);
}

bool HierarchyNode::ContainsChild(QString name){
    for( qint32 index = 0; index < this->GetChildCount(); index++ ){
        if( this->GetChildByIndex(index)->Fit(name) )
            return true;
    }
    return false;
}

void HierarchyNode::UpdateData(qint32 bytes){
    this->packets++;
    this->bytes += bytes;
}

qint32 HierarchyNode::GetPackets(){
    return this->packets;
}

qint32 HierarchyNode::GetBytes(){
    return this->bytes;
}

QString HierarchyNode::GetName(){
    return this->name;
}

bool HierarchyNode::Fit(QString name){
    if( this->name == name )
        return true;
    return false;
}
