#ifndef IOGRAPHCHART_H
#define IOGRAPHCHART_H

#include <QObject>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>

#include "../../../capturer/capturer.h"

using namespace QtCharts;

class IOGraphChart:public QChartView
{
    Q_OBJECT
public:
    IOGraphChart(Capturer *capturer);
    void SetInterval(qint32 ms);

protected:
    void statics();
    void updateChart();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QChart *mychart;
    QLineSeries *series;
    QValueAxis *axisX;
    QValueAxis *axisY;

    QPoint mousePoint;
    Capturer *capturer;
    qreal lastPacketTime;
    bool unInitMaxCount;
    qint32 maxCount;
    qint32 interval;  //单位ms
    QList<qint32> packetCount;
};

#endif // IOGRAPHCHART_H
