#include "SequenceNode.h"

NodeStatus SequenceNode::tick() {
    for (auto& child : children) {
        NodeStatus status = child->tick();
        if (status != NodeStatus::Success) {
            return status;
        }
    }
    return NodeStatus::Success;
}