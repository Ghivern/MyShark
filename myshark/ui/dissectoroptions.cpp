#include "dissectoroptions.h"
#include "ui_dissectoroptions.h"





/*默认选项,所有处理的协议均要添加*/
QHash<QString,quint64>* DissectorOptions::dissectorOptions = new QHash<QString,quint64>
      {
            {"ether",5}
            ,{"arp",6}
            ,{"ipv4",7}
            ,{"tcp",100}
      };

DissectorOptions::DissectorOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DissectorOptions)
{
    ui->setupUi(this);
    this->setWindowTitle("Options");
    this->setupListWidget();
    this->setupWidget();
}

DissectorOptions::~DissectorOptions()
{
    delete ui;
}

void DissectorOptions::setupListWidget(){
    this->ui->listWidget->addItem("ether");
    this->ui->listWidget->addItem("arp");
    this->ui->listWidget->addItem("ipv4");
    this->ui->listWidget->addItem("tcp");
}

void DissectorOptions::setupWidget(){
    this->Vlayout = new QVBoxLayout;
    this->ui->widget->setLayout(Vlayout);

    this->ether = new Ether;
    this->arp = new Arp;

    this->Vlayout->addWidget(ether);
    this->Vlayout->addWidget(arp);

    this->clearWidget();
}

void DissectorOptions::clearWidget(){
    for(qint32 index = 0; index < this->ui->widget->children().length(); index++){
        ((QWidget*)this->ui->widget->children().at(index))->hide();
    }
}

void DissectorOptions::on_listWidget_itemClicked(QListWidgetItem *item)
{
    this->clearWidget();
    if( item->text() == "ether" ){
        this->ether->show();
        *(this->dissectorOptions->find("tcp")) = 600;
    }else if( item->text() == "arp" ){
        this->arp->show();
    }
}
