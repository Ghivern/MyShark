#include "http.h"

Http::Http(ProTree *proTree,DissectResultHttp *dissectResultHttp,void *reserves)
{
    Q_UNUSED(proTree)
    Q_UNUSED(dissectResultHttp)
    Q_UNUSED(reserves)

    proTree->AddItem("http",QString("Http"));
}
