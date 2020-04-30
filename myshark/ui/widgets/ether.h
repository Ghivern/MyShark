#ifndef ETHER_H
#define ETHER_H

#include <QWidget>

namespace Ui {
class Ether;
}

class Ether : public QWidget
{
    Q_OBJECT

public:
    explicit Ether(QWidget *parent = nullptr);
    ~Ether();

private:
    Ui::Ether *ui;
};

#endif // ETHER_H
