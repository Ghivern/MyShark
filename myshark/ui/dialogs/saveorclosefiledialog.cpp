#include "saveorclosefiledialog.h"
#include "ui_saveorclosefiledialog.h"

SaveOrCloseFileDialog::SaveOrCloseFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveOrCloseFileDialog)
{
    ui->setupUi(this);
    this->status = CONTINUE_WITHOUT_SAVING; /* 0:Continue without saving
                          1:Save and continue
                          2:Cancel
                       */

    this->setWindowTitle("Unsaved packets...");
}

SaveOrCloseFileDialog::~SaveOrCloseFileDialog()
{
    delete ui;
}

int SaveOrCloseFileDialog::exec(){
    this->status = 0;
    return QDialog::exec();
}

void SaveOrCloseFileDialog::SetContentForOpenFile(){
    this->ui->label_question->setText("Do you want to save the captured packets before opening another file?");
}

void SaveOrCloseFileDialog::SetContentForNewCapture(){
    this->ui->label_question->setText("Do you want to save the captured packets before starting a new capture?");
}

void SaveOrCloseFileDialog::SetContentForQuit(){
    this->ui->label_question->setText("Do you want to save the captured packets before quiting?");
}

void SaveOrCloseFileDialog::on_pushButton_continue_without_save_clicked()
{
    this->status = CONTINUE_WITHOUT_SAVING;
    emit continueWithoutSave();
    this->accept();
}

void SaveOrCloseFileDialog::on_pushButton_save_clicked()
{
    this->status = CONTINUE_WITH_SAVING;
    emit saveFileBeforeCapture();
    this->reject();
}

void SaveOrCloseFileDialog::on_pushButton_cancle_clicked()
{
    this->status = CANCEL;
    this->reject();
}

bool SaveOrCloseFileDialog::IsContinueWithoutSaving(){
    if( this->status == CONTINUE_WITHOUT_SAVING )
        return true;
    return false;
}

bool SaveOrCloseFileDialog::IsContinueWithSaving(){
    if( this->status == CONTINUE_WITH_SAVING )
        return true;
    return false;
}

bool SaveOrCloseFileDialog::IsCancel(){
    if( this->status == CANCEL )
        return true;
    return false;
}
