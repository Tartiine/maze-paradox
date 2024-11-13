#pragma once
#include "Node.h"
#include "CompositeNode.h"

class SelectorNode : public CompositeNode {
public:
    NodeStatus tick() override {
        for (auto& child : children) {
            NodeStatus status = child->tick();
            if (status != NodeStatus::Failure) {
                return status;
            }
        }
        return NodeStatus::Failure;
    }
};
