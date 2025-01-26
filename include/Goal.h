#ifndef GOAL_H
#define GOAL_H

#include <string>
#include "WorldState.h"

using namespace std;

class Goal {
    string name;

    // Methods
    Goal(const string& name, float priority);
    void addCondition(function<bool(const WorldState&)> function);

    bool isReached(const WorldState& state) const;
};

#endif // GOAL_H