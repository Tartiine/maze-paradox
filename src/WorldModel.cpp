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

void WorldModel::addState(const string& key, const WorldValue& value) {
    state.emplace(key, value);
}

vector<Goal> WorldModel::getAllPossibleActions() const {
    vector<Goal> result;
    
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