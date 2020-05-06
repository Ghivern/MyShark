#ifndef DISPLAYFILTER_H
#define DISPLAYFILTER_H

#include <QObject>
#include <QMutex>
#include "../../dissector/dissres/dissectresutltframe.h"

#include "../../dissector/units/tcpinfo.h"

class DisplayFilter
{
public:
    DisplayFilter();

    bool Filte(DissectResultFrame *frame);
    bool SetFilter(QString filterStr);
    bool FilterIsValied(QString filterStr);

    qint64 GetDisplayedCount();


private:
    /*Display Filter字符串基本结构*/
    /*protocol.properity*/

    QString filterStr;
    QMutex mutex;
    qint64 displayedCount;
};

#endif // DISPLAYFILTER_H
