#ifndef CAPTURER_H
#define CAPTURER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "../units/caphandle.h"

#include "../../dissector/dissres/dissreseth.h"

#include "../dissector/dissres/dissectresutltframe.h"

class Capturer:public QThread
{
    Q_OBJECT
public:
    Capturer(QString devName);
    Capturer(CapHandle *capHandle);
    ~Capturer();
    qint32 GetIntLinkType();
    QList<DissRes*>* GetDissResList();

    qint64 GetCount();

    QList<DissectResultFrame*> testList;
    QList<DissectResultBase*> packetList;

protected:
    void run() Q_DECL_OVERRIDE;

private:
    CapHandle *capHandle;
    DissResList_t *dissResList;
    QMutex *mutex;
    bool stop;



signals:
    void onePacketCaptured(qint64 index);
    void onePacketCaptured(DissectResultFrame *frame);

public slots:
    void Start();
    void Stop();
    void Clear();
};

#endif // CAPTURER_H
