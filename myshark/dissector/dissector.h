#ifndef DISSECTOR_H
#define DISSECTOR_H
#include <QObject>
#include "../dissector/loader.h"
#include "../capturer/capturer.h"

class Dissector:public QObject
{
    Q_OBJECT
public:
    Dissector(DissResList_t* dissResList, int linkType);
    DissRes* GetDissResByIndex(qint64 index);
    Loader* GetLoader(); // -
    DissResList_t* GetDissResList();

private:
    Loader *loader;
    Capturer *capturer; //-
    QList<DissRes*> *dissResList;
    int linkType;

signals:
    void onePacketDissected(qint64 index);

public slots:
    void Dissect(qint64 index);
};

#endif // DISSECTOR_H
