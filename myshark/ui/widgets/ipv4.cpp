#include "ipv4.h"
#include "ui_ipv4.h"

Ipv4::Ipv4(quint64 options,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ipv4)
{
    ui->setupUi(this);

    this->ui->checkBox_validate_checksum->setChecked( options & IPV4_VALIDATE_CHECKSUM ? true : false );
}

Ipv4::~Ipv4()
{
    delete ui;
}

quint64 Ipv4::Result(){
    quint64 options = 0;
    if( this->ui->checkBox_validate_checksum->isChecked() )
        options |= IPV4_VALIDATE_CHECKSUM;
    return options;
}
