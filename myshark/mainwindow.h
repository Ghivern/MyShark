#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QListWidgetItem>
#include <QLabel>

#include "capturer/capturer.h"
#include "dissector/dissector.h"
#include "../dissector/units/device.h"


//ui
#include "./ui/devicelist.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void PrintProTree(ProTreeNode *proTreeNode,qint32 level = 1);

private:
    Ui::MainWindow *ui;

    Capturer *capturer;
    Dissector *dissector;
    Loader *loader;

    QLabel *status;

    enum COL_NAME_VAL{
        COL_NO,
        COL_TIME,
        COL_SOURCE,
        COL_DESTINATION,
        COL_PROTOCOL,
        COL_LENGTH,
        COL_INFO
    };

    void setupUi();
    void setupSignal();


    bool scrollToBottom;



public slots:
    void StartCapture(QListWidgetItem *item);
    void Print(qint64 index);

    void addToTable(DissectResultFrame *frame);
private slots:
    void on_actionStop_triggered();
    void on_actionStart_triggered();
    void on_actionRestart_triggered();
};
#endif // MAINWINDOW_H
