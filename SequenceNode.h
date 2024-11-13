#pragma once
#include "CompositeNode.h"

class SequenceNode : public CompositeNode {
public:
    NodeStatus tick() override {
        for (auto& child : children) {
            NodeStatus status = child->tick();
            if (status != NodeStatus::Success) {
                return status;
            }
        }
        return NodeStatus::Success;
    }
};
