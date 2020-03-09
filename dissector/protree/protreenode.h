#ifndef PROTREENODE_H
#define PROTREENODE_H
#include <QString>

class ProTreeNode
{
public:
    ProTreeNode(QString protocol,QString msg,qint32 start,qint32 end);
    void SetNext(ProTreeNode *next);
    void SetNextLevel(ProTreeNode *nextLevel);
    QString GetMsg();
    ProTreeNode* GetNext();
    ProTreeNode* GetNextLevel();
    qint32 GetStart();
    qint32 GetEnd();

private:
    QString protocol;
    QString msg;
    ProTreeNode *next;
    ProTreeNode *nextLevel;
    qint32 start;
    qint32 end;
};

#endif // PROTREENODE_H
