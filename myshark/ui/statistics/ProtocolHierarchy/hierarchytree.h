#ifndef HIERARCHYTREE_H
#define HIERARCHYTREE_H

#include "hierarchynode.h"
#include "../../../../dissector/dissres/dissectresutltframe.h"

class HierarchyTree
{
public:
    HierarchyTree();
    HierarchyNode* GetHeader();
    void UpdateTree(DissectResultFrame *frame);

private:
    HierarchyNode *header;

};

#endif // HIERARCHYTREE_H
