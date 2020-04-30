#include "mainwindow.h"
#include "./ui/devicelist.h"
#include "./ui/dissectoroptions.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DeviceList deviceList;
    if(deviceList.exec() == QDialog::Accepted){
        MainWindow w(DissectorOptions::dissectorOptions);
        w.show();
        return a.exec();
    }
//    MainWindow w;
//    w.show();
//    return a.exec();
}
