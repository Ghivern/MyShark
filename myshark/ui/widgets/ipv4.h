#ifndef IPV4_H
#define IPV4_H

#include <QWidget>

#include "../../../dissector/dissres/dissectresultbase.h"

namespace Ui {
class Ipv4;
}

class Ipv4 : public QWidget
{
    Q_OBJECT

public:
    explicit Ipv4(quint64 options,QWidget *parent = nullptr);
    ~Ipv4();

    quint64 Result();
private:
    Ui::Ipv4 *ui;
};

#endif // IPV4_H
