#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>

#include "capturer/capturer.h"
#include "dissector/dissector.h"
#include "../dissector/units/device.h"

/*Dissectors -- for 测试*/
#include "../dissector/dissectors/protreemaker.h"


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

private:
    void setupUi();
    void setupSignal();

    Ui::MainWindow *ui;

    Capturer *capturer;
    Dissector *dissector;
    Loader *loader;

    /*StatuBar*/
    QLabel *displayProportion;

    /*tableWidget*/
    bool scrollToBottom;
    enum COL_NAME_VAL{
        COL_NO,
        COL_TIME,
        COL_SOURCE,
        COL_DESTINATION,
        COL_PROTOCOL,
        COL_LENGTH,
        COL_INFO
    };

    /*rawDataPanel*/
    /* |8|1|8|1|16 */
    const qint32 rawDataPanelRowCount = 34;


public slots:
    void Print(qint64 index);
    void PrintProTree(ProTreeNode *proTreeNode,qint32 level = 1);

private slots:
    void addToTable(DissectResultFrame *frame);
    void ergoditTree(QTreeWidgetItem *parent,ProTreeNode *node);
    void addToTree(qint64 index);
    void addToRawDataPanel(qint64 index);

    void on_actionStop_triggered();
    void on_actionStart_triggered();
    void on_actionRestart_triggered();
    void on_tableWidget_cellClicked(int row, int column);
};
#endif // MAINWINDOW_H
