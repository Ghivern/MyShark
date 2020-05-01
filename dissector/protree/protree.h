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
    // 默认start = -1 ,end = -1
    void AddItem(QString protocol,QString msg,qint32 position = level::CURRENT);

    //指定start和end
    void AddItem(QString protocol,QString msg,qint32 start,qint32 end,qint32 position = level::CURRENT);

    //指定start和字段长；  addStart用于控制start指向的的值是否增加qint32(len)/len-1，默认不加;   position用于指定协议树是否分杈，默认不分杈
    void AddItem(QString protocol,QString msg,qint32 *start,float len,bool addStart=false,qint32 position = level::CURRENT);

    void AddNextLayerItem(QString protocol,QString msg,qint32 *start,float len,bool addStart = false);

    void AddCurrentLayerItem(QString protocol,QString msg,qint32 *start,float len,bool addStart = false);

    // void AddItemL(QString protocol,QString msg,qint32 start,float len,qint32 position = level::CURRENT);

    void Pop(qint32 times = 1);
    ProTreeNode* GetHeader();
private:

    //指定start和字段长；  addStart用于控制start指向的的值是否增加qint32(len)/len-1，默认不加;   position用于指定协议树是否分杈，默认不分杈
    void addItem(QString protocol,QString msg,qint32 *start,float len,bool addStart=false,qint32 position = level::CURRENT);

    QStack<ProTreeNode*> nodeStack;
    ProTreeNode *header;
};

#endif // PROTREE_H
