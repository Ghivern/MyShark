#ifndef ETHER_H
#define ETHER_H

#include <QWidget>
#include "../dissector/dissres/dissectresutltframe.h"

namespace Ui {
class Ether;
}

class Ether : public QWidget
{
    Q_OBJECT

public:
    explicit Ether(quint64 option,QWidget *parent = nullptr);
    ~Ether();

    quint64 Result();

private:
    Ui::Ether *ui;
    quint64 option;
};

#endif // ETHER_H
