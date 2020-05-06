#ifndef DISPLAYTABLE_H
#define DISPLAYTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>

#include "../../../dissector/dissres/dissectresutltframe.h"

#include "../../units/displayfilter.h"


class DisplayTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit DisplayTable(QWidget *parent = nullptr);
    ~DisplayTable();

    void SetScrollToBottom(bool scrollToBottom);
    bool SetFilter(QString filterStr);
    DisplayFilter* GetDisplayFilter();
    qint64 GetDisplayedPacketCount();
    void Clear();
    void SetFontPointSize(qint32 pointSize = 0);
    void ResizeTableToFitContent();

public slots:
    void slot_addToTable(DissectResultFrame *frame);

private:
    qint32 addToTable(DissectResultFrame *frame);
    void addStatusColor(DissectResultFrame *frame,qint32 row);

    void setTableWidgetColor(qint32 row,quint32 background, quint32 textColor);
    void setTableWidgetColor(qint32 row,quint32 background);

    enum COL_NAME_VAL{
        COL_NO = 0,
        COL_TIME,
        COL_SOURCE,
        COL_DESTINATION,
        COL_PROTOCOL,
        COL_LENGTH,
        COL_INFO,

        COL_COUNT
    };

    bool scrollToLastRow;

    DisplayFilter *displayFilter;

private slots:
    void slot_rowSelected(qint32 row,qint32 column);

signals:
    void rowSelected(DissectResultFrame *frame);
    void showItemsCountChange(qint32 showItemsCount,qint32 allItemsCount);

};

#endif // DISPLAYTABLE_H
