#ifndef CONVERTER_H
#define CONVERTER_H

#include <QtCore>
#include <QString>

/*
 * 适用于网络字节序的quint8 Array
 * 若转换主机字节序的数组，则打印顺序从低字节开始，会导致与预期结果相反
 */

class Converter
{
public:
    Converter(const quint8 *ptr,qint32 size,QString sep = "",QString preFix = "0x",qint32 base = 16);

    QString ConvertQuint8ArrayToHexStr();
    QString ConvertQuint8ArrayToDecStr();


    static QString ConvertQuint8ArrayToHexStr(const quint8 *ptr,qint32 size,QString sep = "",QString preFix = "0x");
    static QString ConvertQuint8ArrayToDecStr(const quint8 *ptr,qint32 size,QString sep = "",QString preFix = "");    

private:

    static QString ConvertQuint8ArrayToStr(const quint8 *ptr,qint32 size,QString sep,QString preFix,qint32 base);

    const quint8 *ptr;
    qint32 size;
    qint32 base;
    QString sep;
    QString preFix;
};

#endif // CONVERTER_H
