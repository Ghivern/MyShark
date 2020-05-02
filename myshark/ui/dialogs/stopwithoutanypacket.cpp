#include "stopwithoutanypacket.h"
#include "ui_stopwithoutanypacket.h"

StopWithoutAnyPacket::StopWithoutAnyPacket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StopWithoutAnyPacket)
{
    ui->setupUi(this);
    this->setWindowTitle("MyShark");
}

StopWithoutAnyPacket::~StopWithoutAnyPacket()
{
    delete ui;
}

void StopWithoutAnyPacket::on_pushButton_ok_clicked()
{
    this->reject();
}
