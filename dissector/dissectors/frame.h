#ifndef FRAME_H
#define FRAME_H

#include <QtCore>
#include <QCryptographicHash>

#include "../protree/protree.h"

#include "../dissres/dissectresutltframe.h"

//#include "../../dissector/units/dissectorDefaultOptions.h"

class Frame
{
public:
    Frame(ProTree *proTree,DissectResultFrame *dissectResultFrame,void *reserves);

};

#endif // FRAME_H
