#ifndef CAPHANDLE_H
#define CAPHANDLE_H
#include "../../global/global.h"
#include "../../dissector/units/device.h"

class CapHandle
{
public:
    CapHandle();
    CapHandle(QString devName);
    CapHandle(qint32 devIndex);

    void createPcapHandle(QString devName);
    void createPcapHandle(qint32 devIndex);

    qint32 SetSnaplen(qint32 snap);
    qint32 SetPromisc(qint32 promisc);
    qint32 SetImmediateMode(qint32 immediateMode);

    qint32 ActivateHandle();
    qint32 ActivateHandleWithParas(qint32 snapLen = 65535,qint32 promisc = 1,qint32 immediateMode = 1);

    pcap_t *GetPcapHandle();

    qint32 GetLinkType();
    QString GetLinkTypeName();
    QString GetLinkTypeDes();

    QString GetDeviceName();
    qint32 GetDeviceIndex();

private:
    pcap_t *pcapHandle;
    QString devName;
    qint32 devIndex;
};

#endif // CAPHANDLE_H
