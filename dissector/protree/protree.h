#ifndef PROTREE_H
#define PROTREE_H
#include <QStack>
#include "protreenode.h"

class ProTree
{
public:
    typedef enum level{
        CURRENT = 0,
        NEW = 1,
    }level;
    ProTree();
    void AddItem(QString protocol,QString msg,qint32 position = level::CURRENT);
    void AddItem(QString protocol,QString msg,qint32 start,qint32 end,qint32 position = level::CURRENT);
    void AddItem(QString protocol,QString msg,qint32 *start,float len,qint32 position = level::CURRENT);
    void AddItemL(QString protocol,QString msg,qint32 start,float len,qint32 position = level::CURRENT);
    void Pop(qint32 times = 1);
    ProTreeNode* GetHeader();
private:

    QStack<ProTreeNode*> nodeStack;
    ProTreeNode *header;
};

#endif // PROTREE_H
