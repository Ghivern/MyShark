#ifndef CAPTURER_H
#define CAPTURER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "../units/caphandle.h"

#include "../../dissector/dissres/dissreseth.h"

class Capturer:public QThread
{
    Q_OBJECT
public:
    Capturer(QString devName);
    ~Capturer();
    qint32 GetIntLinkType();
    QList<DissRes*>* GetDissResList();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    CapHandle *capHandle;
    DissResList_t *dissResList;
    QMutex *mutex;
    bool stop;

signals:
    void onePacketCaptured(qint64 index);

public slots:
    void Start();
    void Stop();
};

#endif // CAPTURER_H
