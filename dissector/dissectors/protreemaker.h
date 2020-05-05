#ifndef PROTREEMAKER_H
#define PROTREEMAKER_H

#include "../protree/protree.h"

#include "../dissres/dissectresutltframe.h"

#include "tcpipprotocolfamily.h"

class ProTreeMaker
{
public:
    ProTreeMaker(qint32 datalink,DissectResultFrame *dissectRestltFrame,void *reserves);
    ProTree* GetProTree();


private:
    ProTree *proTree;


};

#endif // PROTREEMAKER_H
