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

    void SetData(qint64 allPacketCount,qint64 displayedPacketClunt);

private:
    Ui::DisplayProportion *ui;
};

#endif // DISPLAYPROPORTION_H
