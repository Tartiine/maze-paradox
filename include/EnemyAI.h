#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <iostream>
#include "Player.h"
#include "BehaviorTree.h"
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "TaskNode.h"

class EnemyAI {
public:
    
    EnemyAI(Player& player);

    void update(float dt);

private:
    Player& player;
    std::unique_ptr<BehaviorTree> behaviorTree;

    int jumpCount = 0;
    float jumpTimer = 0.0f;
    const float jumpTimeLimit = 15.0f;  
    const int jumpThreshold = 10;

    void initializeBehaviorTree();

    NodeStatus FastJumper();
    NodeStatus ChangePlayerPhysics();
    NodeStatus Wait();
};

#endif //ENEMYAI_H