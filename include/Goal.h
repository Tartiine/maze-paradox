#ifndef GOAL_H
#define GOAL_H

#include <string>
#include "WorldState.h"

using namespace std;

struct Goal {
    string name;
    vector<function<bool(const WorldState&)>> conditions;
    float priority;

    // Methods
    Goal(const string& name, float priority);
    void addCondition(function<bool(const WorldState&)> function);
};

#endif // GOAL_H