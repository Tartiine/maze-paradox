#ifndef GOAPPLANNER_H
#define GOAPPLANNER_H

#include "Action.h"
#include "Action.h"
#include "Goal.h"

class GOAPPlanner {
    public:
        GOAPPlanner() = default;
        ~GOAPPlanner() = default;

        vector<Action> generatePlan(const WorldState& currentState, const Goal& goal, const vector<Action>& actions);
    
    private:
        struct Node {
            WorldState state;
            float cost;
            vector<Action> actions;
        };
        
        bool isGoalSatisfied(const WorldState& state, const Goal& goal) const;
        float calculateHeuristic(const WorldState& state, const Goal& goal) const;
        vector<Node> getNeighbors(const Node& node, const vector<Action>& actions) const;
};

#endif // GOAPPLANNER_H