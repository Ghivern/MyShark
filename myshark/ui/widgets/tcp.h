#ifndef TCP_H
#define TCP_H

#include <QWidget>

#include "../../../dissector/dissres/dissectresultbase.h"

namespace Ui {
class Tcp;
}

class Tcp : public QWidget
{
    Q_OBJECT

public:
    explicit Tcp(quint64 options,QWidget *parent = nullptr);
    ~Tcp();

    quint64 Result();

private:
    Ui::Tcp *ui;
};

#endif // TCP_H
