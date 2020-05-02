#include "saveorclosefiledialog.h"
#include "ui_saveorclosefiledialog.h"

SaveOrCloseFileDialog::SaveOrCloseFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveOrCloseFileDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Unsaved packets...");
}

SaveOrCloseFileDialog::~SaveOrCloseFileDialog()
{
    delete ui;
}


void SaveOrCloseFileDialog::on_pushButton_continue_without_save_clicked()
{
    emit continueWithoutSave();
    this->accept();
}

void SaveOrCloseFileDialog::on_pushButton_save_clicked()
{
    emit saveFileBeforeCapture("~/");
    this->reject();
}

void SaveOrCloseFileDialog::on_pushButton_cancle_clicked()
{
    this->reject();
}


