#include "dissectoroptions.h"
#include "ui_dissectoroptions.h"

#include "../../dissector/dissres/dissectresultbase.h"


DissectorOptions::DissectorOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DissectorOptions)
{
    ui->setupUi(this);
    this->setWindowTitle("Options");
    this->setupListWidget();
    this->dissectorOptions = DissectResultBase::GetDissectorOptionPtr();
    this->setupWidget();
}

DissectorOptions::~DissectorOptions()
{
    delete ui;
}

void DissectorOptions::setupListWidget(){
    this->ui->listWidget->addItem("frame");
    this->ui->listWidget->addItem("ether");
    this->ui->listWidget->addItem("arp");
    this->ui->listWidget->addItem("ipv4");
    this->ui->listWidget->addItem("tcp");

    this->ui->listWidget->setCurrentRow(0);
}

void DissectorOptions::setupWidget(){
    this->Vlayout = new QVBoxLayout;
    this->ui->widget->setLayout(Vlayout);

    this->frame = new Frame(this->dissectorOptions->value("frame"));
    this->ether = new Ether(this->dissectorOptions->value("ether"));
    this->arp = new Arp;

    this->Vlayout->addWidget(frame);
    this->Vlayout->addWidget(ether);
    this->Vlayout->addWidget(arp);

    this->clearWidget();
    this->frame->show();
}

void DissectorOptions::clearWidget(){
    for(qint32 index = 0; index < this->ui->widget->children().length(); index++){
        ((QWidget*)this->ui->widget->children().at(index))->hide();
    }
}

void DissectorOptions::on_listWidget_itemClicked(QListWidgetItem *item)
{
    this->clearWidget();
    if( item->text() == "frame" ){
        this->frame->show();
    }else if( item->text() == "ether" ){
        this->ether->show();
//        *(this->dissectorOptions->find("tcp")) = 600;
    }else if( item->text() == "arp" ){
        this->arp->show();
    }
}

void DissectorOptions::on_pushButton_ok_clicked()
{
    (*dissectorOptions->find("frame")) = this->frame->Result();
    (*dissectorOptions->find("ether")) = this->ether->Result();

    this->accept();
}

void DissectorOptions::on_pushButton_cancel_clicked()
{
    this->reject();
}
