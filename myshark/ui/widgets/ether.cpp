#include "ether.h"
#include "ui_ether.h"
#include <QtDebug>

Ether::Ether(quint64 option,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ether)
{
    ui->setupUi(this);
    this->option = option;
    this->ui->checkBox_validate_fcs->setChecked(
                (option & ETHERNET_VALIDATE_FCS ? true:false)
                );
}

Ether::~Ether()
{
    delete ui;
}

quint64 Ether::Result(){
    quint64 options = 0;
    if( this->ui->checkBox_validate_fcs->isChecked() )
        options |= ETHERNET_VALIDATE_FCS;
    return options;
}
