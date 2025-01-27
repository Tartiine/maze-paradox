#ifndef GOAP_PLANNER_H
#define GOAP_PLANNER_H

#include "Action.h"
#include "float.h"
#include <memory>

using namespace std;

class GOAPPlanner {
    public:
        struct GOAPNode {
            float worldDiscontentment = FLT_MAX;
            bool isValid = false;
            shared_ptr<Action> action;
        };

        GOAPPlanner(int maxDepth);
        ~GOAPPlanner();

        void resetPlanner();
        GOAPNode getPlannedAction(int depth, const WorldModel& model);

    private:
        int maxDepth;
};

#endif // GOAP_PLANNER_H