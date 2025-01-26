#include "Action.h"

Action::Action(const string& name, float cost) : name(name), preconditions(), effects() {}

void Action::addPreconditions(function<bool(const WorldState&)> function)
{
    preconditions.emplace_back(function);
}

void Action::addEffects(function<void(WorldState&)> function)
{
    effects.emplace_back(function);
}

bool Action::canExecute(const WorldState& state) const {
    for (const auto& precondition : preconditions) {
        if (!precondition(state)) {
            return false;
        }
    }
    return true;
}

void Action::execute(WorldState& state) const {
    for (const auto& effect : effects) {
        effect(state);
    }
}