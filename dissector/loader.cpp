#include "loader.h"

Loader::Loader()
{
    this->dissectorHash.insert(1,new DissectorEth());
}

DissectorBase* Loader::GetDissector(qint32 datalinktype){
    return this->dissectorHash.value(datalinktype);
}
