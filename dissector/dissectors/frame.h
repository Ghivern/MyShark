#ifndef FRAME_H
#define FRAME_H

#include <QtCore>

#include "../protree/protree.h"

#include "../dissres/dissectresutltframe.h"

class Frame
{
public:
    Frame(ProTree *proTree,DissectResultFrame *dissectResultFrame,void *reserves);

};

#endif // FRAME_H
