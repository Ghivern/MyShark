#ifndef DISSECTOR_H
#define DISSECTOR_H
#include <QObject>
#include "../global/global.h"
#include "../dissector/loader.h"
#include "../capturer/capturer.h"

class Dissector:public QObject
{
    Q_OBJECT
public:
    Dissector(Capturer *capturer,Loader *loader);
    ~Dissector();
    DissRes* GetDissResByIndex(qint64 index);
    Loader* GetLoader();
    dissResList_t* GetDissResList();

private:
    dissResList_t *dissResList;
    Loader *loader;
    Capturer *capturer;

signals:
    void onePacketDissected(qint64 index);

public slots:
    void Dissect(qint64 index);
};

#endif // DISSECTOR_H
