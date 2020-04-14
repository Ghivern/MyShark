#ifndef CONVERTER_H
#define CONVERTER_H

#include <QtCore>
#include <QString>

class Converter
{
public:
    Converter(quint8 *ptr,qint32 size,QString sep = "",QString preFix = "0x",qint32 base = 16);

    QString ConvertQuint8ArrayToHexStr();
    QString ConvertQuint8ArrayToDecStr();


    static QString ConvertQuint8ArrayToHexStr(quint8 *ptr,qint32 size,QString sep = "",QString preFix = "0x");
    static QString ConvertQuint8ArrayToDecStr(quint8 *ptr,qint32 size,QString sep = "",QString preFix = "0x");

private:

    static QString ConvertQuint8ArrayToStr(quint8 *ptr,qint32 size,QString sep,QString preFix,qint32 base);

    quint8 *ptr;
    qint32 size;
    qint32 base;
    QString sep;
    QString preFix;
};

#endif // CONVERTER_H
