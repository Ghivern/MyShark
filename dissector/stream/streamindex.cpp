#include "streamindex.h"

StreamIndex::StreamIndex()
{

}

//private
qint32 StreamIndex::exist(QString addressS,QString addressD,quint16 portS,quint16 portD,qint64 *streamIndex){
    *streamIndex = -1;
    QCryptographicHash Hash(QCryptographicHash::Md5);
    QString str = addressS+QString::asprintf("%ud",portS)+addressD+QString::asprintf("%ud",portD);
    Hash.addData(str.toLocal8Bit());
    QByteArray md5SD = Hash.result();

    if(streamIndex_hash.contains(md5SD)){
        *streamIndex = streamIndex_hash.value(md5SD);
        return 0;
    }else{
        str = addressD+QString::asprintf("%ud",portD)+addressS+QString::asprintf("%ud",portS);
        Hash.reset();
        Hash.addData(str.toLocal8Bit());
        QByteArray md5DS = Hash.result();
        if(streamIndex_hash.contains(md5DS)){
            *streamIndex = streamIndex_hash.value(md5DS);
            return -1;
        }else{
            return -1;
        }
    }
}

qint64 StreamIndex::Add(QString addressS,QString addressD,quint16 portS,quint16 portD, qint64 streamItem_index){
    QCryptographicHash Hash(QCryptographicHash::Md5);
    QString str = addressS+QString::asprintf("%ud",portS)+addressD+QString::asprintf("%ud",portD);
    Hash.addData(str.toLocal8Bit());
    QByteArray md5SD = Hash.result();
    qint64 index;
    if(this->exist(addressS,addressD,portS,portD,&index) == -1){
        if(index==-1){
            streamIndex_hash.insert(md5SD,streamItem_index);
            return streamItem_index;
        }else{
            streamIndex_hash.insert(md5SD,index);
        }
    }
    return index;
}

qint64 StreamIndex::GetStreamItemIndex(QString addressS,QString addressD,quint16 portS,quint16 portD){
    QCryptographicHash Hash(QCryptographicHash::Md5);
    QString str = addressS+QString::asprintf("%ud",portS)+addressD+QString::asprintf("%ud",portD);
    Hash.addData(str.toLocal8Bit());
    QByteArray md5SD = Hash.result();
    if(streamIndex_hash.contains(md5SD))
        return streamIndex_hash.value(md5SD);
    else
        return -1;
}
