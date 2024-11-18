#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <iostream>

class EnemyAI {
public:
    EnemyAI();
    ~EnemyAI();

    void update();

private:
    void processPhysics();
    void checkProximity();
    void reactToPlayer();
    void manageAIState();
    void initializeBehaviorTree();
};

#endif //ENEMYAI_H