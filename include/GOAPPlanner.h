#ifndef GOAP_PLANNER_H
#define GOAP_PLANNER_H

#include "Action.h"
#include <memory>

using namespace std;

class GOAPPlanner {
    public:
        GOAPPlanner();
        ~GOAPPlanner();

        void resetPlanner();
        Action getPlannedAction(const WorldModel& model);

    private:
        float bestDiscontentment;
        unique_ptr<Action> bestAction;
};

#endif // GOAP_PLANNER_H