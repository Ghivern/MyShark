#ifndef DISPLAYRAWDATA_H
#define DISPLAYRAWDATA_H

#include <QTabWidget>
#include <QTableWidgetItem>

#include "../../../dissector/dissres/dissectresutltframe.h"

class DisplayRawData : public QTableWidget
{
    Q_OBJECT
public:
    explicit DisplayRawData(QWidget *parent = nullptr);
    void Clear();
    void SetFontPointSize(qint32 pointSize = 0);

public slots:
    void slot_addToRawDataPanel(DissectResultFrame *frame);
    void slot_hightLigth(qint32 start,qint32 end);

private:
    void addToRawDataPanel(DissectResultFrame *frame);
    void clearRawDataPanelBackground(quint32 background);
    void hightLight(qint32 start, qint32 end);

    const qint32 rawDataPanelColCount = 34;
};

#endif // DISPLAYRAWDATA_H
