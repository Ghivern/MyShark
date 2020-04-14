#include "converter.h"

Converter::Converter(const quint8 *ptr,qint32 size,QString sep,QString preFix,qint32 base){
    this->ptr = ptr;
    this->size = size;
    this->base = base;
    this->sep.append(sep);
    this->preFix.append(preFix);
}

QString Converter::Converter::ConvertQuint8ArrayToDecStr(){
    return Converter::ConvertQuint8ArrayToStr(this->ptr,this->size,this->sep,this->preFix,10);
}

QString Converter::ConvertQuint8ArrayToHexStr(){
    return Converter::ConvertQuint8ArrayToStr(this->ptr,this->size,this->sep,this->preFix,16);
}


QString Converter::ConvertQuint8ArrayToDecStr(const quint8 *ptr, qint32 size, QString sep, QString preFix){
    return Converter::ConvertQuint8ArrayToStr(ptr,size,sep,preFix,10);
}

QString Converter::ConvertQuint8ArrayToHexStr(const quint8 *ptr, qint32 size, QString sep, QString preFix){
    return Converter::ConvertQuint8ArrayToStr(ptr,size,sep,preFix,16);
}


/*Private*/
QString Converter::ConvertQuint8ArrayToStr(const quint8 *ptr, qint32 size, QString sep, QString preFix, qint32 base){
    QString convertRes(preFix);
    for(qint32 index = 0; index < size; index++){
        convertRes.append(QString("%1").arg(ptr[index],base==10?0:2,base,QLatin1Char('0')));
        if(index!=size - 1)
            convertRes.append(sep);
    }
    return convertRes;
}
