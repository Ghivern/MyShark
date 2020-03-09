#include "protree.h"

ProTree::ProTree()
{

}

void ProTree::AddItem(QString protocol,QString msg,qint32 start,qint32 end,qint32 position){
    if(msg.length() != 0){
        ProTreeNode *newNode = new ProTreeNode(protocol,msg,start,end);
        if(this->nodeStack.isEmpty()){
            this->nodeStack.push(newNode);
            this->header = newNode;
        }else{
            if(position == level::CURRENT){
                this->nodeStack.pop()->SetNext(newNode);
                this->nodeStack.push(newNode);
            }else if(position == level::NEW){
                this->nodeStack.top()->SetNextLevel(newNode);
                this->nodeStack.push(newNode);
            }else{
                while (position < 0) {
                    this->nodeStack.pop();
                    position++;
                }
                this->nodeStack.pop()->SetNext(newNode);
                this->nodeStack.push(newNode);
            }
        }
    }
}

void ProTree::AddItem(QString protocol, QString msg, qint32 position){
    if(msg.length() != 0){
        qint32 start = -1;
        qint32 end = -1;
        ProTreeNode *newNode = new ProTreeNode(protocol,msg,start,end);
        if(this->nodeStack.isEmpty()){
            this->nodeStack.push(newNode);
            this->header = newNode;
        }else{
            if(position == level::CURRENT){
                this->nodeStack.pop()->SetNext(newNode);
                this->nodeStack.push(newNode);
            }else if(position == level::NEW){
                this->nodeStack.top()->SetNextLevel(newNode);
                this->nodeStack.push(newNode);
            }else{
                while (position < 0) {
                    this->nodeStack.pop();
                    position++;
                }
                this->nodeStack.pop()->SetNext(newNode);
                this->nodeStack.push(newNode);
            }

        }
    }
}

void ProTree::Pop(qint32 times){
    for(qint32 index = 0; index < times; index++)
        this->nodeStack.pop();
}

ProTreeNode* ProTree::GetHeader(){
    return this->header;
}