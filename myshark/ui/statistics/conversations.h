#ifndef CONVERSATIONS_H
#define CONVERSATIONS_H

#include <QWidget>

namespace Ui {
class Conversations;
}

class Conversations : public QWidget
{
    Q_OBJECT

public:
    explicit Conversations(QWidget *parent = nullptr);
    ~Conversations();

private:
    Ui::Conversations *ui;
};

#endif // CONVERSATIONS_H
