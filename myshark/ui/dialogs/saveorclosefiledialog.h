#ifndef SAVEORCLOSEFILEDIALOG_H
#define SAVEORCLOSEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class SaveOrCloseFileDialog;
}

class SaveOrCloseFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveOrCloseFileDialog(QWidget *parent = nullptr);
    ~SaveOrCloseFileDialog();

private:
    Ui::SaveOrCloseFileDialog *ui;

signals:
    void saveFileBeforeCapture(QString path);
    void continueWithoutSave();

private slots:
    void on_pushButton_continue_without_save_clicked();
    void on_pushButton_cancle_clicked();
    void on_pushButton_save_clicked();
};

#endif // SAVEORCLOSEFILEDIALOG_H
