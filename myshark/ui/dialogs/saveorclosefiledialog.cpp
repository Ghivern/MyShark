#include "saveorclosefiledialog.h"
#include "ui_saveorclosefiledialog.h"

SaveOrCloseFileDialog::SaveOrCloseFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveOrCloseFileDialog)
{
    ui->setupUi(this);
}

SaveOrCloseFileDialog::~SaveOrCloseFileDialog()
{
    delete ui;
}
