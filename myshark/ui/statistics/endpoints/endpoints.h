#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <QWidget>

#include "endpointstablewidget.h"

namespace Ui {
class Endpoints;
}

class Endpoints : public QWidget
{
    Q_OBJECT

public:
    explicit Endpoints(Capturer *capturer,QWidget *parent = nullptr);
    ~Endpoints();

private:
    Ui::Endpoints *ui;
};

#endif // ENDPOINTS_H
