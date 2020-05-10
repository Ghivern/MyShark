#ifndef CONVERSATIONTABLEWIDGET_H
#define CONVERSATIONTABLEWIDGET_H

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

#include "../../../../dissector/stream/stream.h"
#include "../../../capturer/capturer.h"

class ConversationTableWidget:public QTableWidget
{
public:
    ConversationTableWidget(Stream &stream,Capturer *capturer,bool linklayer = false,QWidget *parent = nullptr);

private:
    void addToTable();
//    QStringList headers = {"Address A","Address B","Packets","Bytes",
//                           "Packets A->B","Bytes A->B","Packets B->A","Bytes B->A",
//                           "Relative Start","Duration","Bits/s A->B","Bits/s B->A"};
   enum POS_WITH_PORT{
        A_ADDR = 0,
        A_PORT,
        B_ADDR,
        B_PORT,
        PACKETS,
        BYTES,
        A_B_PACKETS,
        A_B_BYTES,
        B_A_PACKETS,
        B_A_BYTES,
        REL_START,
        DURATION,
        A_B_BITS_PER_SECOND,
        B_A_BITS_PER_SECOND
    };

    Stream &stream;
    Capturer *capturer;
    bool linklayer;
};

#endif // CONVERSATIONTABLEWIDGET_H
