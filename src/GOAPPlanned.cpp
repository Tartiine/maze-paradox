#include "GOAPPlanner.h"

GOAPPlanner::GOAPPlanner(int maxDepth) : maxDepth(maxDepth) {}

GOAPPlanner::GOAPNode GOAPPlanner::getPlannedAction(int depth, const WorldModel& model) {
    GOAPPlanner::GOAPNode bestNode;

    vector<Action> possibleActions = model.getAllPossibleActions();

    for (auto& action : possibleActions) {
        WorldModel worldModelCopy = WorldModel(model);

        if (action.applyAction(worldModelCopy)) {
            if (depth + 1 >= maxDepth) {
                float discontentment = worldModelCopy.getWorldDiscontentment();

                if (discontentment < bestNode.worldDiscontentment) {
                    
                }
            }
        }
    }

    return bestNode;
}