#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QScrollBar>
#include <QWheelEvent>

#include "capturer/capturer.h"
#include "dissector/dissector.h"
#include "../dissector/units/device.h"

/*Dissectors -- for 测试*/
#include "../dissector/dissectors/protreemaker.h"


//ui
#include "./ui/devicelist.h"
#include "./ui/dissectoroptions.h"
#include "./ui/dialogs/saveorclosefiledialog.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QHash<QString,quint64> *dissectorOptions,QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUi();
    void setupSignal();
    void setupDissectorOptions();

    void setTableWidgetColor(qint32 row,quint32 background, quint32 text);
    void setTableWidgetColor(qint32 row,quint32 background);
    void clearRawDataPanelBackground(quint32 background);

    void addBackgroundToTableRow(DissectResultFrame *frame,qint32 row);

    bool eventFilter(QObject *target, QEvent *event);

    Ui::MainWindow *ui;

    Capturer *capturer;
    //Dissector *dissector;
    //Loader *loader;

    /*StatuBar*/
    QLabel *displayProportion;

    /*tableWidget*/
    bool scrollToBottom;
    const qint32 tableWidgetColCount = 7;
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
    const qint32 rawDataPanelColCount = 34;
    const qint32 defaultTextSize = 10;


    /*解析选项，用协议名作为Key*/
    QHash<QString,quint64> *dissectorOptions;

    /*暂时用于存储过滤器需要的StreamIndex*/
    qint64 streamIndex;


public slots:
    void Print(qint64 index);
    void PrintProTree(ProTreeNode *proTreeNode,qint32 level = 1);

private slots:
    void addToTable(DissectResultFrame *frame);
    void ergoditTree(QTreeWidgetItem *parent,ProTreeNode *node);
    void addToTree(qint64 index);
    void addToRawDataPanel(qint64 index);


    void on_tableWidget_cellClicked(int row, int column);
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_actionStop_triggered();
    void on_actionStart_triggered();
    void on_actionRestart_triggered();
    void on_actionEnlargeTextSize_triggered();
    void on_actionShrinkTextSize_triggered();
    void on_actionDefaultTextSize_triggered();
    void on_actionScrollToLastLine_triggered(bool checked);
    void on_actionResizeTableWidgetTOFitContents_triggered();
    void on_pushButton_filter_clicked();
    void on_actionDissector_options_triggered();
};
#endif // MAINWINDOW_H
