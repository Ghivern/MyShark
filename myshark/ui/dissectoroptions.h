#ifndef DISSECTOROPTIONS_H
#define DISSECTOROPTIONS_H

#include <QDialog>
#include <QListWidgetItem>
#include <QVBoxLayout>

#include "./widgets/frame.h"
#include "./widgets/ether.h"
#include "./widgets/arp.h"

#include "../../dissector/units/dissectorDefaultOptions.h"

namespace Ui {
class DissectorOptions;
}

class DissectorOptions : public QDialog
{
    Q_OBJECT

public:
    //static QHash<QString,quint64> *dissectorOptions;


    explicit DissectorOptions(QWidget *parent = nullptr);
    ~DissectorOptions();
    void setupListWidget();
    void setupWidget();

    void clearWidget();

private:
    Ui::DissectorOptions *ui;

    QVBoxLayout *Vlayout;

    QHash<QString,quint64>* dissectorOptions;

    Frame *frame;
    Arp *arp;
    Ether *ether;


private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
};

#endif // DISSECTOROPTIONS_H
