#ifndef DISSECTOROPTIONS_H
#define DISSECTOROPTIONS_H

#include <QDialog>
#include <QListWidgetItem>
#include <QVBoxLayout>

#include "./widgets/frame.h"
#include "./widgets/ether.h"
#include "./widgets/arp.h"
#include "./widgets/ipv4.h"
#include "./widgets/tcp.h"
#include "./widgets/udp.h"

//#include "../../dissector/units/dissectorDefaultOptions.h"

namespace Ui {
class DissectorOptions;
}

class DissectorOptions : public QDialog
{
    Q_OBJECT

public:

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
    Ether *ether;
    Arp *arp;
    Ipv4 *ipv4;
    Tcp *tcp;
    Udp *udp;




private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
};

#endif // DISSECTOROPTIONS_H
