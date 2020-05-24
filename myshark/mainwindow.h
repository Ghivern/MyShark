#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMessageBox>
#include <QFileDialog>

#include <sys/stat.h>

#include "capturer/capturer.h"
#include "dissector/dissector.h"
#include "../dissector/units/device.h"

/*Dissectors -- for 测试*/
#include "../dissector/dissectors/protreemaker.h"


//ui
//#include "./ui/devicelist.h"
#include "./ui/dissectoroptions.h"
#include "./ui/dialogs/saveorclosefiledialog.h"
#include "./ui/dialogs/stopwithoutanypacket.h"
#include "./ui/statusbar/displayproportion.h"

//Statistics
#include "./ui/statistics/capturefileproperties.h"
#include "./ui/statistics/conversations/conversations.h"
#include "./ui/statistics/endpoints/endpoints.h"
#include "./ui/statistics/packetlengths/packetlength.h"
#include "./ui/statistics/ProtocolHierarchy/protocolhierarchy.h"
#include "./ui/statistics/iograph/iographchart.h"

//display filter
#include "./units/displayfilter.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
   void closeEvent(QCloseEvent *event);

private:
    void setupUi();
    void setupSignal();

    enum WINDOW_STATUS{
        STATUS_NONE,
        STATUS_CAPTURING,
        STATUS_STOP,
        STATUS_FILE,
    };
    void setActionsStatus(enum WINDOW_STATUS status = STATUS_NONE);

    bool eventFilter(QObject *target, QEvent *event);

    Ui::MainWindow *ui;

    //Statistics
    //CaptureFileProperties *captureFileProperties;

    Capturer *capturer;

    /*StatuBar*/
    DisplayProportion *showDisplayProportion;
    const qint32 defaultTextSize = 10;





private slots:
    /*控制启止*/
    void slot_start(QString interfaceOrFile,bool fromFile);

    void on_actionStart_triggered();
    void on_actionStop_triggered();
    void on_actionRestart_triggered();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionClose_triggered();
    void on_actionQuit_triggered();

    /*显示过滤器*/
    void on_lineEdit_textChanged(const QString &arg1);
    void on_pushButton_filter_clicked();


    /*改变Window的Title*/
    void slot_changeWindowTitle();

    /*控制显示效果*/
    void on_actionEnlargeTextSize_triggered();
    void on_actionShrinkTextSize_triggered();
    void on_actionDefaultTextSize_triggered();
    void on_actionScrollToLastLine_triggered(bool checked);
    void on_actionResizeTableWidgetTOFitContents_triggered();

    /*解析器选项面板*/
    void on_actionDissector_options_triggered();

    /*统计结果显示面板*/
    void on_actionCapture_file_properitys_triggered();
    void on_actionConversations_triggered();

    //public slots:
    //    void Print(qint64 index);
    //    void PrintProTree(ProTreeNode *proTreeNode,qint32 level = 1);


    void on_actionactionEndpoints_triggered();
    void on_actionPacketLengths_triggered();
    void on_actionProtocolHierarchy_triggered();
    void on_actionIOGraph_triggered();
};
#endif // MAINWINDOW_H
