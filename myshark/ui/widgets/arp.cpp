#include "arp.h"
#include "ui_arp.h"

Arp::Arp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Arp)
{
    ui->setupUi(this);
}

Arp::~Arp()
{
    delete ui;
}

quint64 Arp::Result(){
    return 200;
}
