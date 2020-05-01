#ifndef DEVICELIST_H
#define DEVICELIST_H

#include <QDialog>
#include <QListWidgetItem>

#include "../units/caphandle.h"

namespace Ui {
class DeviceList;
}

class DeviceList:public QDialog
{
    Q_OBJECT
public:
    explicit DeviceList(QWidget *parent = nullptr);
    ~DeviceList();

    static QString SelectedDevice;
    static CapHandle *capHandle;

private:
    Ui::DeviceList *ui;

    void setupDeviceList();
    void setupSignal();

private slots:
    void updateSelectedDevice(QListWidgetItem *item);

    void on_buttonBox_accepted();
    void on_pushButton_options_clicked();
};

#endif // DEVICELIST_H
