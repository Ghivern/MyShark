#ifndef HTTP_H
#define HTTP_H

#include "../../protree/protree.h"
#include "../../dissres/dissectresutltframe.h"

class Http
{
public:
    Http(ProTree *proTree,DissectResultHttp *dissectResultHttp,void *reserves);
};

#endif // HTTP_H
