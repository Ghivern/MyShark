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
    void saveFileBeforeStartingNewCapture(QString path);
};

#endif // SAVEORCLOSEFILEDIALOG_H
