#ifndef SEQUENCE_NODE_H
#define SEQUENCE_NODE_H

#include "CompositeNode.h"

class SequenceNode : public CompositeNode {
public:
    NodeStatus tick() override;
};

#endif // SEQUENCE_NODE_H