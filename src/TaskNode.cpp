#include "TaskNode.h"

using namespace std;

TaskNode::TaskNode(const string& taskName, function<NodeStatus()> func)
        : name(taskName), taskFunction(move(func)) {}

NodeStatus TaskNode::tick() {
    return taskFunction();
}