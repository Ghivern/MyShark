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
    bool FilteWithoutLogicOperator(DissectResultFrame *frame);
    //bool FilteWithLogicOperator(DissectResultFrame *frame);
    bool SetFilter(QString filterStr);
    bool FilterIsValied(QString filterStr);

    //bool subExpressionIsValied(QString subExpression);

    qint64 GetDisplayedCount();


private:
    /*Display Filter字符串基本结构*/
    /*protocol.properity*/

    QString filterStr;
    QMutex mutex;
    qint64 displayedCount;


    QStringList valiedCondition =
    {
        "eth"
        ,"ipv4"
        ,"arp"
        ,"tcp"
        ,"udp"
        ,"ipv4.addr"
        ,"ipv4.saddr"
        ,"ipv4.daddr"
        ,"tcp.port"
        ,"tcp.sport"
        ,"tcp.dport"
        ,"udp.port"
        ,"udp.sport"
        ,"udp.dport"
        ,"tcp.stream"
        ,"udp.stream"
    };
};

#endif // DISPLAYFILTER_H
