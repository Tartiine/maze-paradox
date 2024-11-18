#include "TaskNode.h"

TaskNode::TaskNode(const std::string& taskName, bool (*func)())
    : name(taskName), taskFunction(func) {}

NodeStatus TaskNode::tick() {
    std::cout << "Executing task: " << name << std::endl;
    return taskFunction() ? NodeStatus::Success : NodeStatus::Failure;
}