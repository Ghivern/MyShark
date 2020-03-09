#ifndef CAPTURER_H
#define CAPTURER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include "../../global/global.h"
#include "../units/caphandle.h"

class Capturer:public QThread
{
    Q_OBJECT
public:
    Capturer(QString devName);
    ~Capturer();
    rawList_t* GetRawList();

protected:
    void run() Q_DECL_OVERRIDE;

private:
    CapHandle *capHandle;
    rawList_t *rawList;
    QMutex *mutex;
    bool stop;

signals:
    void onePacketCaptured(qint64 index);

public slots:
    void Start();
    void Stop();
};

#endif // CAPTURER_H
