#include "ether.h"
#include "ui_ether.h"

Ether::Ether(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ether)
{
    ui->setupUi(this);
}

Ether::~Ether()
{
    delete ui;
}
