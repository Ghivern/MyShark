#include "displayproportion.h"
#include "ui_displayproportion.h"

DisplayProportion::DisplayProportion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayProportion)
{
    ui->setupUi(this);
    this->ui->label->setText("No Packet");
}

DisplayProportion::~DisplayProportion()
{
    delete ui;
}

void DisplayProportion::SetDefaultInfo(){
    this->ui->label->setText("No Packet");
}


void DisplayProportion::slot_setData(qint32 displayedPacketCount,qint32 allPacketCount){
    this->setData(displayedPacketCount,allPacketCount);
}


void DisplayProportion::setData(qint32 displayedPacketCount,qint32 allPacketCount){
    this->ui->label->setText(
                QString("Packets: %1; Displayed: %2 (%3)")
                .arg(allPacketCount)
                .arg(displayedPacketCount)
                .arg(QString::asprintf("%.1f",displayedPacketCount * 1.0 / allPacketCount * 100) + "%")
                );
}
