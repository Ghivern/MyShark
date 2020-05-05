#include "displayinterfaces.h"

DisplayInterfaces::DisplayInterfaces(QWidget *parent)
    :QListWidget(parent)
{
    this->setStyleSheet("background-color:transparent");
    Device device;
    QListWidgetItem *item;
    for(qint32 index = 0; index < device.GetDeviceCount(); index++){
        item = new QListWidgetItem();
        item->setText(device.GetDevices().at(index));
        this->addItem(item);
    }
    this->setCurrentRow(0);
    this->selectedInterface.clear();
    if( this->item(0) )
        this->selectedInterface.append(this->item(0)->text());

    connect(this,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_updateSelectedInterface(QListWidgetItem*)));
    connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(slot_updateSelectedInterfaceAndEmitSignal(QListWidgetItem*)));
}

QString DisplayInterfaces::GetSelectedInterface(){
    return this->selectedInterface;
}

void DisplayInterfaces::slot_updateSelectedInterface(QListWidgetItem *item){
    this->selectedInterface.clear();
    this->selectedInterface.append(item->text());
}

void DisplayInterfaces::slot_updateSelectedInterfaceAndEmitSignal(QListWidgetItem *item){
    this->slot_updateSelectedInterface(item);
    emit selectedInterfaceChanged(item->text(),false);
}
