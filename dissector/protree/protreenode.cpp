#include "protreenode.h"

ProTreeNode::ProTreeNode(QString protocol,QString msg,qint32 start,qint32 end)
{
    this->start = start;
    this->end = end;
    this->protocol = protocol;
    this->msg = msg;
    this->next = NULL;
    this->nextLevel = NULL;
}

void ProTreeNode::SetNext(ProTreeNode *next){
    this->next = next;
}

void ProTreeNode::SetNextLevel(ProTreeNode *nextLevel){
    this->nextLevel = nextLevel;
}

QString ProTreeNode::GetMsg(){
    return this->msg;
}

ProTreeNode* ProTreeNode::GetNext(){
    return this->next;
}

ProTreeNode* ProTreeNode::GetNextLevel(){
    return this->nextLevel;
}

qint32 ProTreeNode::GetStart(){
    return this->start;
}

qint32 ProTreeNode::GetEnd(){
    return this->end;
}
