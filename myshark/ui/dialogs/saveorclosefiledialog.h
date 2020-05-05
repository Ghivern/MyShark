#ifndef SAVEORCLOSEFILEDIALOG_H
#define SAVEORCLOSEFILEDIALOG_H

#include <QDialog>

#define CONTINUE_WITHOUT_SAVING 0x01
#define CONTINUE_WITH_SAVING 0x02
#define CANCEL 0x04

namespace Ui {
class SaveOrCloseFileDialog;
}

class SaveOrCloseFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveOrCloseFileDialog(QWidget *parent = nullptr);
    ~SaveOrCloseFileDialog();

    void SetContentForOpenFile();
    void SetContentForNewCapture();
    void SetContentForQuit();
    void SetContentForClose();

    bool IsContinueWithoutSaving();
    bool IsContinueWithSaving();
    bool IsCancel();

private:
    Ui::SaveOrCloseFileDialog *ui;
    qint32 status;

signals:
    void saveFileBeforeCapture();
    void continueWithoutSave();

private slots:
    void on_pushButton_continue_without_save_clicked();
    void on_pushButton_cancle_clicked();
    void on_pushButton_save_clicked();
};

#endif // SAVEORCLOSEFILEDIALOG_H
