#ifndef LOADER_H
#define LOADER_H
#include <QHash>
#include "./dissectors/dissectorbase.h"
#include "./dissectors/dissectoreth/dissectoreth.h"
class Loader
{
public:
    Loader();
    DissectorBase* GetDissector(qint32 datalinktype);

private:
    QHash<qint32,DissectorBase*> dissectorHash;

};

#endif // LOADER_H
