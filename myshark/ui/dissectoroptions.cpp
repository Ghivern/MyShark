#include "dissectoroptions.h"
#include "ui_dissectoroptions.h"

#include "../../dissector/dissres/dissectresultbase.h"



///*默认选项,所有处理的协议均要添加*/
//QHash<QString,quint64>* DissectorOptions::dissectorOptions = new QHash<QString,quint64>
//      {
//            {"frame",
//                 FRAME_SHOW_NUMBER_OF_BITS | FRAME_GENERATE_EPOCH_TIME
//            },

//            {"ether",

//            0},

//            {"arp",

//            6},

//            {"ipv4",

//            7},

//            {"tcp",

//            100}
//      };

DissectorOptions::DissectorOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DissectorOptions)
{
    ui->setupUi(this);
    this->setWindowTitle("Options");
    this->setupListWidget();
    this->dissectorOptions = DissectResultBase::DissectorOptions;
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
