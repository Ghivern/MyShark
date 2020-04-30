#ifndef ARP_H
#define ARP_H

#include <QWidget>

namespace Ui {
class Arp;
}

class Arp : public QWidget
{
    Q_OBJECT

public:
    explicit Arp(QWidget *parent = nullptr);
    ~Arp();

    quint64 Result();

private:
    Ui::Arp *ui;
};

#endif // ARP_H
