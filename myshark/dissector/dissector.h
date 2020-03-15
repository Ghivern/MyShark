#ifndef DISSECTOR_H
#define DISSECTOR_H
#include <QObject>
#include "../dissector/loader.h"
#include "../capturer/capturer.h"

class Dissector:public QObject
{
    Q_OBJECT
public:
    Dissector(Capturer *capturer,Loader *loader);
    Dissector(Capturer *capture);
    DissRes* GetDissResByIndex(qint64 index);
    Loader* GetLoader();
    DissResList* GetDissResList();

private:
    Loader *loader;
    Capturer *capturer;
    QList<DissRes*> *dissResList;

signals:
    void onePacketDissected(qint64 index);

public slots:
    void Dissect(qint64 index);
};

#endif // DISSECTOR_H
