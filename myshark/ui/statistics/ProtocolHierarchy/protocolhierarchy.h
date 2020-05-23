#ifndef PROTOCOLHIERARCHY_H
#define PROTOCOLHIERARCHY_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "../../../capturer/capturer.h"
#include "hierarchytree.h"

namespace Ui {
class ProtocolHierarchy;
}

class ProtocolHierarchy : public QWidget
{
    Q_OBJECT

public:
    explicit ProtocolHierarchy(Capturer *capturer, QWidget *parent = nullptr);
    ~ProtocolHierarchy();

private:
    QStringList headers = {
        "Protocol",
        "Packets",
        "Percent Packets",
        "Bytes",
        "Percent Bytes",
        "Bits/s"
    };

    enum COLUMN_POSITIONS{
        PROTOCOL = 0,
        PACKETS,
        PERCENTPACKETS,
        BYTES,
        PERCENTBYTES,
        BITSPERSECOND
    };

    Ui::ProtocolHierarchy *ui;

    Capturer *capturer;
    HierarchyTree *tree;

    qint32 allPackets;
    qint32 allBytes;
    float allTime;

    void addToTree();
    void addToTreeLoop(QTreeWidgetItem *parentItem,HierarchyNode *parentNode);
    void addToTreeItem(QTreeWidgetItem *item,HierarchyNode *node);

};

#endif // PROTOCOLHIERARCHY_H
