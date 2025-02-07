#include "WorldModel.h"

WorldModel::WorldModel() {}

WorldModel::WorldModel(const WorldModel& model) {
    this->state = model.state;
    this->actions = model.actions;
    this->goals = model.goals;
}

void WorldModel::setState(const string& key, const WorldValue& value) {
    state[key] = value;
}

template <typename T>
T WorldModel::getState(const string& key) const {
    auto it = state.find(key);
    if (it != state.end()) {
        return get<T>(it->second);
    }
}

bool WorldModel::has(const string& key) const {
    state.find(key) != state.end();
}

bool WorldModel::isDiscovered() const {
    return isDiscovered;
}

void WorldModel::resetDiscovery() {
    isDiscovered = false;
}

void WorldModel::addState(const string& key, const WorldValue& value) {
    state.emplace(key, value);
}

void WorldModel::addAction(const Action& action) {
    actions.emplace_back(action);
}

void WorldModel::addGoal(const Goal& goal) {
    goals.emplace_back(goal);
}

vector<Action> WorldModel::getAllPossibleActions() const {
    vector<Action> result;
    
    for (auto& action : actions) {
        if (action.canApply(*this)) {
            result.emplace_back(action);
        }
    }

    return result;
}

float WorldModel::getWorldDiscontentment() const {
    float result = 0;

    for (auto& goal : goals) {
        result += goal.getDiscontentment(*this);
    }

    return result;
}