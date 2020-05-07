#ifndef COPY_H
#define COPY_H

#include <QtCore>

class Copy
{
public:
    Copy(quint32 dataLen);
    Copy& Append(const quint8 *src,quint32 len);
    quint8* GetData();

private:
    quint32 startPosition;

    quint32 dataLen;
    quint8 *data;
};

#endif // COPY_H
