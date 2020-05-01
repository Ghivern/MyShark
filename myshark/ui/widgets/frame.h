#ifndef FRAME_H
#define FRAME_H

#include <QWidget>
#include "../dissector/dissres/dissectresutltframe.h"

namespace Ui {
class Frame;
}

class Frame : public QWidget
{
    Q_OBJECT

public:
    explicit Frame(quint64 option,QWidget *parent = nullptr);
    ~Frame();
    quint64 Result();

private:
    Ui::Frame *ui;
};

#endif // FRAME_H
