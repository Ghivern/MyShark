#ifndef DISPLAYTREE_H
#define DISPLAYTREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "../../../dissector/protree/protree.h"

#include "../../../dissector/dissres/dissectresutltframe.h"
#include "../../../dissector/dissectors/protreemaker.h"

class DisplayTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit DisplayTree(QWidget *parent = nullptr);
    void Clear();
    void SetFontPointSize(qint32 pointSize = 0);

public slots:
    void slot_addToTree(DissectResultFrame *frame);

private:
    void ergoditTree(QTreeWidgetItem *parent,ProTreeNode *node);
    void addToTree(DissectResultFrame *frame);

private slots:
    void slot_itemSelected(QTreeWidgetItem *item,qint32 column);

signals:
    void oneItemSelected(qint32 start,qint32 end);
};

#endif // DISPLAYTREE_H
