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

void DisplayProportion::SetData(qint64 allPacketCount, qint64 displayedPacketCount){
    this->ui->label->setText(
                QString("Packets: %1; Displayed: %2 (%3)")
                .arg(allPacketCount)
                .arg(displayedPacketCount)
                .arg(QString::asprintf("%.1f",displayedPacketCount * 1.0 / allPacketCount * 100) + "%")
                );
}
