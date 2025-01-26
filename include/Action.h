#ifndef ACTION_H
#define ACTION_H

#include <string>
#include "WorldState.h"

using namespace std;

struct Action {
    string name;
    vector<function<bool(const WorldState&)>> preconditions;
    vector<function<void(WorldState&)>> effects;
    float cost;

    // Methods
    Action(const string& name, float cost);
    void addPreconditions(function<bool(const WorldState&)> function);
    void addeffects(function<void(WorldState&)> function);

    bool canExecute(const WorldState& state) const;
    void execute(WorldState& state) const;
};

#endif // ACTION_H