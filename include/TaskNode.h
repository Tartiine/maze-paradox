#ifndef TASK_NODE_H
#define TASK_NODE_H

#include <string>
#include <iostream>
#include "Node.h"  
#include <functional>


class TaskNode : public Node {
private:
    std::string name;
    std::function<NodeStatus()> taskFunction;
public:
    TaskNode(const std::string& taskName, std::function<NodeStatus()> func);
    NodeStatus tick() override;
};

#endif // TASK_NODE_H