#include "Goal.h"

Goal::Goal(const string& name, float priority) : name(name) {}

void Goal::addCondition(function<bool(const WorldState&)> function) {
    conditions.emplace_back(function);
}

bool Goal::isReached(const WorldState& state) const {
    for (const auto& condition : conditions) {
        if (!condition(state)) {
            return false;
        }
    }
    return true;
}