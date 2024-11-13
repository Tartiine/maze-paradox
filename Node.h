#pragma once

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
