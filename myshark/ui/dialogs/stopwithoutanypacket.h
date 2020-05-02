#ifndef STOPWITHOUTANYPACKET_H
#define STOPWITHOUTANYPACKET_H

#include <QDialog>

namespace Ui {
class StopWithoutAnyPacket;
}

class StopWithoutAnyPacket : public QDialog
{
    Q_OBJECT

public:
    explicit StopWithoutAnyPacket(QWidget *parent = nullptr);
    ~StopWithoutAnyPacket();

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::StopWithoutAnyPacket *ui;
};

#endif // STOPWITHOUTANYPACKET_H
