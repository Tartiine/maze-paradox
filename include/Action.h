#ifndef ACTION_H
#define ACTION_H

#include "WorldModel.h"

class Action {
    public:
        Action();
        ~Action() = default;

        virtual bool canApply(const WorldModel& model) const = 0;                 // pre-conditions
        virtual bool applyAction(WorldModel& model) = 0;                          // effects

    protected:
        string name;
};

#endif // ACTION_H