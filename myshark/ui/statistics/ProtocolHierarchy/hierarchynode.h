#ifndef HIERARCHYNODE_H
#define HIERARCHYNODE_H

#include <QList>

class HierarchyNode
{
public:
    HierarchyNode(QString name);

    qint32 GetChildCount();
    HierarchyNode* GetChildByIndex(qint32 index);
    HierarchyNode* GetChildByName(QString name);
    void AppendChild(HierarchyNode *newChild);
    bool ContainsChild(QString name);

    void UpdateData(qint32 bytes);

    qint32 GetPackets();
    qint32 GetBytes();
    QString GetName();

    bool Fit(QString name);

private:
    QString name;
    qint32 packets;
    qint32 bytes;

    QList<HierarchyNode*> children;
};

#endif // HIERARCHYNODE_H
