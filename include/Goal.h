#ifndef GOAL_H
#define GOAL_H

#include "WorldModel.h"

class Goal {
    public:
        Goal();
        ~Goal() = default;

        virtual float getDiscontentment(const WorldModel& model) const = 0;
    
    protected:
        string name;
};

#endif // GOAL_H