#ifndef CAPHANDLE_H
#define CAPHANDLE_H
#include "../../dissector/units/device.h"

class CapHandle
{
public:
    CapHandle(QString devNameOrPath,bool fromFile = false);


    void SetSnaplen(qint32 snap);
    void SetPromisc(qint32 promisc);
    void SetImmediateMode(qint32 immediateMode);
    void SetNonBlock(qint32 nonblock);

    void ActivateHandle();
    void ActivateHandleWithParas(qint32 promisc = 1,qint32 immediateMode = 1,qint32 snapLen = 65535);

    void Close();

    bool FromFile();

    pcap_t *GetPcapHandle();

    qint32 GetLinkType();
    QString GetLinkTypeName();
    QString GetLinkTypeDes();

    QString GetDeviceName();
    qint32 GetDeviceIndex();
    QString GetFileName();

private:
    void createHandle();

    bool fromFile;

    pcap_t *pcapHandle;
    QString devNameOrPath;
    qint32 devIndex;
};

#endif // CAPHANDLE_H
