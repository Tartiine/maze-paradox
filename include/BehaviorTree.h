#ifndef BEHAVIOR_TREE_H
#define BEHAVIOR_TREE_H

#include "Node.h"
#include <memory>

class BehaviorTree {
private:
    std::unique_ptr<Node> rootNode;

public:
    BehaviorTree(std::unique_ptr<Node> root);
    void setRoot(std::unique_ptr<Node> root);
    void run();
};

#endif // BEHAVIOR_TREE_H