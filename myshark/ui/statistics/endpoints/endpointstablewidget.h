#ifndef ENDPOINTSTABLEWIDGET_H
#define ENDPOINTSTABLEWIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QCryptographicHash>

#include "../../../../dissector/stream/stream.h"
#include "../../../capturer/capturer.h"

class EndpointsTableWidget:public QTableWidget
{
public:
    EndpointsTableWidget(Stream &stream,Capturer *capturer,bool linklayer = false,bool tcp = false,QWidget *parent = nullptr);

private:
    void startStatics();
    void addToTable();

    enum POS_WITH_PORT{
        ADDR = 0,
        PORT,
        PACKETS,
        BYTES,
        S_PACKETS,
        S_BYTES,
        R_PACKETS,
        R_BYTES
    };

    Stream &stream;
    Capturer *capturer;
    bool linklayer;
    bool tcp;

    //为统计数据而设计的数据结构
    typedef struct endPoint{
        QString address;
        quint16 port;
        qint64 receivedPackets;
        qint64 receivedBytes;
        qint64 sendPackets;
        qint64 sendBytes;
    }endPoint_t;

    QHash<QString,qint64> indexes; //QString::Port的Hash作为Key
    QList<endPoint_t> results;
};

#endif // ENDPOINTSTABLEWIDGET_H
