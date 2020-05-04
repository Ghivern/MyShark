#include "capturefileproperties.h"
#include "ui_capturefileproperties.h"

CaptureFileProperties::CaptureFileProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CaptureFileProperties)
{
    ui->setupUi(this);
    this->text.clear();
}

CaptureFileProperties::~CaptureFileProperties()
{
    delete ui;
}


void CaptureFileProperties::Clear(){
    this->text.clear();
}

void CaptureFileProperties::SetFile(QString path,QString encapsulation){
    QString fileText("<b>File</b>"
                       "<p>"
                       "<pre>Name:                   %1</pre>"
                       "<pre>Length:                 %2 bytes</pre>"
                       "<pre>Format:                 %3</pre>"
                       "<pre>Encapsulation:          %4</pre>"
                       "</p>"
                       "<br>");
    QFile aFile(path);
    this->text.append(
             fileText
            .arg(path)
            .arg(aFile.size())
            .arg("pcap")
            .arg(encapsulation)
                );
}

void CaptureFileProperties::SetTime(qint64 firstPacketTime, qint64 lastPacketTime){
    QString timeText("<b>Time</b>"
                    "<p>"
                    "<pre>First packet:          %1</pre>"
                    "<pre>Last packet:           %2</pre>"
                    "<pre>Elapsed:               %3</pre>"
                    "</p>"
                    "<br>");

    QDateTime firstPacketDateTime = QDateTime::fromSecsSinceEpoch(firstPacketTime);
    QDateTime lastPacketDateTime = QDateTime::fromSecsSinceEpoch(lastPacketTime);
    QTime elapsed((lastPacketTime - firstPacketTime)/3600
                      ,(lastPacketTime - firstPacketTime)%3600/60
                      ,(lastPacketTime - firstPacketTime)%60);
    this->text.append(timeText.arg(firstPacketDateTime.toString("yyyy-MM-dd  hh:mm:ss"))
                .arg(lastPacketDateTime.toString("yyyy-MM-dd  hh:mm:ss"))
                .arg(elapsed.toString("hh:mm:ss"))
                      );
}

void CaptureFileProperties::SetCapture(){
    QProcess process(this);
    process.setProgram("lscpu");
    process.open();
    process.waitForFinished();
    QString cpuInfo = process.readAll().split('\n').at(12).split(':').at(1).trimmed();

    QString captureText("<b>Capture</b>"
                        "<p>"
                        "<pre>Hardware:          %1</pre>"
                        "<pre>OS:                %2 %3</pre>"
                        "</p>"
                        "<br>");
    this->text.append(captureText
            .arg(cpuInfo)
            .arg(QSysInfo::kernelType())
            .arg(QSysInfo::kernelVersion())
            );
}

void CaptureFileProperties::SetInterface(QString interfaceName, qint32 droppedPackets
                                         , QString captureFilter, QString linktypeName, qint32 packetSizeLimit){

    QString interfaceText("<b>Interface</b>"
                          "<p>"
                          "<pre>Interface     Dropped packets     Capture filter     Link type     Packet size limit</pre>"
                          "<pre> %1%2%3%4%5</pre>"
                          "</p>"
                          "<br>");

    this->text.append(
                interfaceText
                .arg(interfaceName,8,' ')
                .arg(QString("%1").arg(droppedPackets),20,' ')
                .arg(captureFilter,18,' ')
                .arg(linktypeName,15,' ')
                .arg(QString("%1").arg(packetSizeLimit),22,' ')
                );
}

void CaptureFileProperties::SetText(){
    this->ui->textEdit->setHtml(this->text);
}
