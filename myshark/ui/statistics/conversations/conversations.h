#ifndef CONVERSATIONS_H
#define CONVERSATIONS_H

#include <QWidget>

#include "conversationtablewidget.h"

namespace Ui {
class Conversations;
}

class Conversations : public QWidget
{
    Q_OBJECT

public:
    explicit Conversations(Capturer *capturer,QWidget *parent = nullptr);
    ~Conversations();

private:
    Ui::Conversations *ui;
};

#endif // CONVERSATIONS_H
