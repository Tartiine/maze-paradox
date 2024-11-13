#pragma once

#include <string>
#include <iostream>
#include "Node.h"  // Assuming Node is part of the hierarchy

class TaskNode : public Node {
private:
    std::string name;
    bool (*taskFunction)();
public:
    TaskNode(const std::string& taskName, bool (*func)()) 
        : name(taskName), taskFunction(func) {}

    NodeStatus tick() override {
        std::cout << "Executing task: " << name << std::endl;
        return taskFunction() ? NodeStatus::Success : NodeStatus::Failure;
    }
};
