#include "CompositeNode.h"

void CompositeNode::addChild(std::shared_ptr<Node> child) {
    children.push_back(child);
}
