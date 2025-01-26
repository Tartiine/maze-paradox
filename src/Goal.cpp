#include "Goal.h"

Goal::Goal(const string& name, float priority) : name(name), conditions(), priority(priority) {}

void Goal::addCondition(function<bool(const WorldState&)> function) {
    conditions.emplace_back(function);
}