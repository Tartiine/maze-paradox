#include "GOAPPlanner.h"

GOAPPlanner::GOAPPlanner(int maxDepth) : maxDepth(maxDepth) {}

GOAPPlanner::GOAPNode GOAPPlanner::getPlannedAction(int depth, const WorldModel& model) {
    GOAPNode bestNode;

    vector<Action> possibleActions = model.getAllPossibleActions();

    for (auto& action : possibleActions) {
        WorldModel worldModelCopy = WorldModel(model);

        if (action.applyAction(worldModelCopy)) {
            if (depth + 1 >= maxDepth) {
                float discontentment = worldModelCopy.getWorldDiscontentment();

                if (discontentment < bestNode.worldDiscontentment) {
                    bestNode.action = make_shared<Action>(action);
                    bestNode.worldDiscontentment = discontentment;
                    bestNode.isValid = true;
                }
            } else {
                GOAPNode nextBestNode = getPlannedAction(depth+1, worldModelCopy);

                if (nextBestNode.worldDiscontentment < bestNode.worldDiscontentment) {
                    bestNode.worldDiscontentment = nextBestNode.worldDiscontentment;
                    bestNode.action = make_shared<Action>(action);
                    bestNode.isValid = true;
                }
            }
        }
    }

    return bestNode;
}