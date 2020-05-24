#include "iographchart.h"

IOGraphChart::IOGraphChart(Capturer *capturer)
{

    this->capturer = capturer;
    this->interval = 1000;
    this->maxCount = 0;
    this->unInitMaxCount = true;
    this->lastPacketTime = this->capturer->GetCount() == 0 ? 0 : this->capturer->GetDissectResultFrameByIndex(this->capturer->GetCount() - 1)->GetRelativeTimeSinceFirstPacket();

    this->setMouseTracking(true);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setAutoFillBackground(true);

    this->mychart = new QChart();
    this->mychart->setTitle("IO Graph");
    this->setChart(mychart);

    this->series = new QLineSeries();
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(Qt::black);
    this->series->setPen(pen);
    this->chart()->addSeries(series);

    this->axisX = new QValueAxis();
    this->axisX->setTitleText("Time(s)");
    this->axisX->setTickInterval(1.5);
    this->axisY = new QValueAxis();
    this->axisY->setTitleText("Packets/s");
    this->axisY->setTickInterval(20);

    this->chart()->addAxis(this->axisX,Qt::AlignBottom);
    this->chart()->addAxis(this->axisY,Qt::AlignLeft);
    this->series->attachAxis(this->axisX);
    this->series->attachAxis(this->axisY);

    this->SetInterval(1000);
}

void IOGraphChart::SetInterval(qint32 ms){
    this->interval = ms;
    this->packetCount.clear();

    qint32 num = qCeil(this->lastPacketTime * 1000.0 / this->interval);
    for( qint32 index = 0; index < num; index++ )
        this->packetCount.append(0);

    this->statics();

    this->axisX->setRange(0,qCeil(this->lastPacketTime));
    this->axisY->setRange(0,qCeil(this->maxCount * 1.0 / 20) * 20);

    this->updateChart();
}

void IOGraphChart::statics(){
    qint32 position = 0;
    for( qint32 index = 0; index < this->capturer->GetCount(); index++ ){
        position =
            qFloor(
                this->capturer->GetDissectResultFrameByIndex(index)->GetRelativeTimeSinceFirstPacket() * 1000.0 / this->interval
            );
        if( position < this->packetCount.length() ){
            this->packetCount[position]++;
            if( this->unInitMaxCount )
                this->maxCount = this->maxCount > packetCount.at(position) ? this->maxCount : packetCount.at(position);
        }

    }
    this->unInitMaxCount = false;
}

void IOGraphChart::updateChart(){
    this->series->clear();
    for( qint32 index = 0; index < this->packetCount.length(); index++ ){
        this->series->append((index + 1) * this->interval / 1000.0,this->packetCount.at(index));
    }
}

void IOGraphChart::mousePressEvent(QMouseEvent *event){
    if( event->button() == Qt::LeftButton ){
        this->mousePoint = event->pos();
    }
    QChartView::mousePressEvent(event);
}

void IOGraphChart::mouseMoveEvent(QMouseEvent *event){
    if( event->button() == Qt::LeftButton ){
        QPoint point = event->pos();
        this->chart()->scroll(point.x() - this->mousePoint.x(),point.y() - this->mousePoint.y());
        QChartView::mouseMoveEvent(event);
    }else if( event->button() == Qt::MiddleButton ){
        ;
    }
}

void IOGraphChart::mouseReleaseEvent(QMouseEvent *event){
    if( event->button() == Qt::LeftButton ){
        this->mousePoint = event->pos();
    }
    QChartView::mouseReleaseEvent(event);
}
