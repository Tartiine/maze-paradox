#ifndef SELECTOR_NODE_H
#define SELECTOR_NODE_H

#include "Node.h"
#include "CompositeNode.h"

class SelectorNode : public CompositeNode {
public:
    NodeStatus tick() override;
};

#endif // SELECTOR_NODE_H