#include "frame.h"
#include "ui_frame.h"

Frame::Frame(quint64 option,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);

    this->ui->checkBox_generate_md5_hash->setChecked((option & FRAME_GENERATE_MD5_HASH ? true:false));
    this->ui->checkBox_generate_epoch_time->setChecked((option & FRAME_GENERATE_EPOCH_TIME ? true:false));
    this->ui->checkBox_show_number_of_bits->setChecked((option & FRAME_SHOW_NUMBER_OF_BITS ? true:false));
}

Frame::~Frame()
{
    delete ui;
}

quint64 Frame::Result(){
    quint64 option = 0;
    if( this->ui->checkBox_generate_md5_hash->isChecked() )
        option |= FRAME_GENERATE_MD5_HASH;
    if( this->ui->checkBox_generate_epoch_time->isChecked() )
        option |= FRAME_GENERATE_EPOCH_TIME;
    if( this->ui->checkBox_show_number_of_bits->isChecked() )
        option |= FRAME_SHOW_NUMBER_OF_BITS;
    return option;
}
