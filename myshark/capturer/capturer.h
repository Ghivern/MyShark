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
    Capturer(QString devName,QHash<QString,quint64>* dissectorOptions = nullptr);
    Capturer(CapHandle *capHandle,QHash<QString,quint64>* dissectorOptions = nullptr);
    ~Capturer();
//    qint32 GetIntLinkType();
//    QList<DissRes*>* GetDissResList();

    qint64 GetCount();

    CapHandle* GetCapHandle();
    DissectResultFrame* GetDissectResultFrameByIndex(qint64 index);



protected:
    void run() Q_DECL_OVERRIDE;

private:
    CapHandle *capHandle;
    //DissResList_t *dissResList;
//    QMutex *mutex;
    bool stop;

    QList<DissectResultFrame*> dissectResultFrameList;

    QHash<QString,quint64>* dissectorOptions;

signals:
    //void onePacketCaptured(qint64 index);
    void onePacketCaptured(DissectResultFrame *frame);

public slots:
    void Start();
    void Stop();
};

#endif // CAPTURER_H
