#ifndef CAPTURER_H
#define CAPTURER_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTemporaryFile>
#include <QFileDialog>
#include <QMessageBox>

#include "../ui/dialogs/saveorclosefiledialog.h"
#include "../ui/dialogs/stopwithoutanypacket.h"

#include "../units/caphandle.h"
#include "../units/dumper.h"

//#include "../../dissector/dissres/dissreseth.h"

#include "../dissector/dissres/dissectresutltframe.h"

class Capturer:public QThread
{
    Q_OBJECT
public:
    Capturer();
    ~Capturer();

    qint64 GetCount();
    CapHandle* GetCapHandle();
    DissectResultFrame* GetDissectResultFrameByIndex(qint64 index);

    QString GetFilePath();

public slots:
    bool slot_startThread(QString interfaceOrFile,bool fromFile);
    bool slot_stopThread();
    bool slot_clearThread(qint32 newCapOrFileOrCloseOrQuit = 2);
    bool slot_saveTempFile();


protected:
    void run() Q_DECL_OVERRIDE;

private:
    bool updateCaptureThread(QString interfaceOrFile,bool fromFile = false);
    bool clearCaptureThread(qint32 newCapOrFileOrQuit = 0);
    bool saveTempFile();

    QMutex mutexForStartAndStop;

    CapHandle *capHandle;
    QTemporaryFile *tempfile;
    Dumper *dumper;

    QHash<QString,quint64>* dissectorOptions;

    bool tempFileHaveBeenSaved;

    QList<DissectResultFrame*> dissectResultFrameList;

signals:
    void onePacketCaptured(DissectResultFrame *frame);
};

#endif // CAPTURER_H
