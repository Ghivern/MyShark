#include "devicelist.h"
#include "ui_devicelist.h"

#include "../../dissector/units/device.h"

#include "dissectoroptions.h"

#include <QListWidgetItem>
#include <QMessageBox>
#include <QDebug>

QString DeviceList::SelectedDevice = "";
CapHandle* DeviceList::capHandle = nullptr;

DeviceList::DeviceList(QWidget *parent)
        :QDialog(parent)
        ,ui(new Ui::DeviceList)
{
    this->ui->setupUi(this);
    this->setupDeviceList();
    this->ui->checkBox->setChecked(true);
    this->setupSignal();
}

DeviceList::~DeviceList(){
    delete this->ui;
}

void DeviceList::setupDeviceList(){
    Device device;
    QListWidgetItem *item;
    for(qint32 index = 0; index < device.GetDeviceCount(); index++){
        item = new QListWidgetItem();
        item->setText(device.GetDevices().at(index));
        this->ui->listWidget->addItem(item);
    }
}

void DeviceList::setupSignal(){
    connect(this->ui->listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(updateSelectedDevice(QListWidgetItem*)));
}

void DeviceList::updateSelectedDevice(QListWidgetItem *item){
    DeviceList::SelectedDevice.clear();
    DeviceList::SelectedDevice.append(item->text());

    try {
        DeviceList::capHandle = new CapHandle(item->text());
        DeviceList::capHandle->ActivateHandleWithParas();
        if( DeviceList::capHandle->GetLinkType() != 1){
            QMessageBox::critical(this,"Error","Corresponding parser has not been added yet");
            return;
        }
    } catch (QString e) {
            QMessageBox::critical(this,"Error",e);
            return;
    }
//    if(DeviceList::capHandle->ActivateHandleWithParas(this->ui->checkBox->isChecked()?1:0) == -1){
//        QMessageBox::critical(this,"Error",DeviceList::capHandle->GetError());
//        return;
//    }else{
//        if(DeviceList::capHandle->GetLinkType() != 1){
//            QMessageBox::critical(this,"Error","Corresponding parser has not been added yet");
//            return;
//        }
//    }
    this->accept();
}

void DeviceList::on_buttonBox_accepted()
{
    QListWidgetItem *item = this->ui->listWidget->currentItem();
    if(item == nullptr)
        QMessageBox::critical(this,"Error","please select a device!");
    else
        this->updateSelectedDevice(item);
}

void DeviceList::on_pushButton_options_clicked()
{
    DissectorOptions dp;
    dp.exec();
}
