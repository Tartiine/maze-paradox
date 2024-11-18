#include "SelectorNode.h"

NodeStatus SelectorNode::tick() {
    for (auto& child : children) {
        NodeStatus status = child->tick();
        if (status != NodeStatus::Failure) {
            return status;
        }
    }
    return NodeStatus::Failure;
}