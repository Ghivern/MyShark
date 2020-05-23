#ifndef PACKETLENGTH_H
#define PACKETLENGTH_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "../../../capturer/capturer.h"

namespace Ui {
class PacketLength;
}

class PacketLength : public QWidget
{
    Q_OBJECT

public:
    explicit PacketLength(Capturer *capturer,QWidget *parent = nullptr);
    ~PacketLength();



private:
    QStringList headers = {
        "Topic/Item",
        "Count",
        "Average",
        "Min Val",
        "Max Val",
        "Rate(ms)",
        "Percent"
    };

    enum COLUMEPOSITION{
        TOPICITEM,
        COUNT,
        AVERAGE,
        MINVAL,
        MAXVAL,
        RATE,
        PERCENT
    };

    typedef struct staticsResult{
        QString topicItem;
        qint32 count;
        qint32 minVal;
        qint32 maxVal;
        qint32 bytes;
    }staticsResult_t;


    void statics(staticsResult *result,qint32 start = 0,qint32 end = -1);
    void addToTreeWidgetItem(staticsResult_t *result,QTreeWidgetItem *item,qint32 totalCount,float totalTime);
    void addToTree();


    Ui::PacketLength *ui;

    Capturer *capturer;
};

#endif // PACKETLENGTH_H
