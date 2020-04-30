#ifndef DISSECTOROPTIONS_H
#define DISSECTOROPTIONS_H

#include <QDialog>
#include <QListWidgetItem>
#include <QVBoxLayout>

#include "./widgets/ether.h"
#include "./widgets/arp.h"

namespace Ui {
class DissectorOptions;
}

class DissectorOptions : public QDialog
{
    Q_OBJECT

public:
    static QHash<QString,quint64> *dissectorOptions;


    explicit DissectorOptions(QWidget *parent = nullptr);
    ~DissectorOptions();
    void setupListWidget();
    void setupWidget();

    void clearWidget();

private:
    Ui::DissectorOptions *ui;

    QVBoxLayout *Vlayout;
    Arp *arp;
    Ether *ether;


private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
};

#endif // DISSECTOROPTIONS_H
