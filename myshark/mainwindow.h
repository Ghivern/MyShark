#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "capturer/capturer.h"
#include "dissector/dissector.h"
#include "../dissector/units/device.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void PrintProTree(ProTreeNode *proTreeNode,qint32 level);

private:
    Ui::MainWindow *ui;

    Capturer *capturer;
    Dissector *dissector;

public slots:
    void Print(qint64 index);
};
#endif // MAINWINDOW_H
