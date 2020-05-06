#include "tcp.h"
#include "ui_tcp.h"

Tcp::Tcp(quint64 options,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tcp)
{
    ui->setupUi(this);

    this->ui->checkBox_validate_checksum->setChecked(options & TCP_VALIDATE_CHECKSUM ? true : false);
    this->ui->checkBox_analyze_seq->setChecked(options & TCP_ANALYZE_TCP_SEQUENCE_NUMBER ? true : false);
    this->ui->checkBox_relative_seq->setChecked(options & TCP_RELATIVE_SEQUENCE_NUMBER ? true : false);
    this->ui->checkBox_ignore_TS_in_summery->setChecked(options & TCP_IGNORE_TIMESTAMPS_IN_SUMMERY ? true : false);
}

Tcp::~Tcp()
{
    delete ui;
}

quint64 Tcp::Result(){
    quint64 options = 0;
    if( this->ui->checkBox_validate_checksum->isChecked() )
        options |= TCP_VALIDATE_CHECKSUM;
    if( this->ui->checkBox_analyze_seq->isChecked() )
        options |= TCP_ANALYZE_TCP_SEQUENCE_NUMBER;
    if( this->ui->checkBox_relative_seq->isChecked() )
        options |= TCP_RELATIVE_SEQUENCE_NUMBER;
    if( this->ui->checkBox_ignore_TS_in_summery->isChecked() )
        options |= TCP_IGNORE_TIMESTAMPS_IN_SUMMERY;
    return options;
}
