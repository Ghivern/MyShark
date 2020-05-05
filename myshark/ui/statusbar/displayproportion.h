#ifndef DISPLAYPROPORTION_H
#define DISPLAYPROPORTION_H

#include <QWidget>

namespace Ui {
class DisplayProportion;
}

class DisplayProportion : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayProportion(QWidget *parent = nullptr);
    ~DisplayProportion();

    void SetDefaultInfo();


public slots:
    void slot_setData(qint32 displayedPacketCount,qint32 allPacketCount);

private:
    void setData(qint32 displayedPacketCount,qint32 allPacketCount);

    Ui::DisplayProportion *ui;
};

#endif // DISPLAYPROPORTION_H
