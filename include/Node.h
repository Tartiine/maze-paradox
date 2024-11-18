#ifndef NODE_H
#define NODE_H

enum class NodeStatus {
    Success,
    Failure,
    Running
};

class Node {
public:
    virtual ~Node() = default;
    virtual NodeStatus tick() = 0;
};

#endif // NODE_H