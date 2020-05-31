#include "udp.h"
#include "ui_udp.h"

Udp::Udp(quint64 options,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Udp)
{
    ui->setupUi(this);

    this->ui->checkBox_validate_checksum->setChecked(options & UDP_VALIDATE_CHECKSUM ? true : false);

}

Udp::~Udp()
{
    delete ui;
}

quint64 Udp::Result(){
    quint64 options = 0;
    if( this->ui->checkBox_validate_checksum->isChecked() )
        options |= TCP_VALIDATE_CHECKSUM;
    return options;
}
