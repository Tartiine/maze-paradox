#ifndef TASK_NODE_H
#define TASK_NODE_H

#include <string>
#include <iostream>
#include "Node.h"  


class TaskNode : public Node {
private:
    std::string name;
    bool (*taskFunction)();
public:
    TaskNode(const std::string& taskName, bool (*func)());
    NodeStatus tick() override;
};

#endif // TASK_NODE_H