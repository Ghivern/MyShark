#ifndef UDP_H
#define UDP_H

#include <QWidget>

#include "../../../dissector/dissres/dissectresultbase.h"

namespace Ui {
class Udp;
}

class Udp : public QWidget
{
    Q_OBJECT

public:
    explicit Udp(quint64 options,QWidget *parent = nullptr);
    ~Udp();

    quint64 Result();

private:
    Ui::Udp *ui;
};

#endif // UDP_H
