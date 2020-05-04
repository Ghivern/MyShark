#ifndef CAPTUREFILEPROPERTIES_H
#define CAPTUREFILEPROPERTIES_H

#include <QWidget>
#include <QTextEdit>
#include <QFile>
#include <QDateTime>
#include <QProcess>
#include <QDebug>


namespace Ui {
class CaptureFileProperties;
}

class CaptureFileProperties : public QWidget
{
    Q_OBJECT

public:
    explicit CaptureFileProperties(QWidget *parent = nullptr);
    ~CaptureFileProperties();

    void Clear();
    void SetFile(QString path,QString encapsulation);
    void SetTime(qint64 firstPacketTime,qint64 lastPacketTime);
    void SetCapture();
    void SetInterface(QString interfaceName,qint32 droppedPackets,QString captureFilter,QString linktypeName,qint32 packetSizeLimit);


    void SetText();

private:
    Ui::CaptureFileProperties *ui;

    QString text;
};

#endif // CAPTUREFILEPROPERTIES_H
