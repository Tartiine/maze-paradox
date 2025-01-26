#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <iostream>
#include "Player.h"
#include "BehaviorTree.h"
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "TaskNode.h"
#include "TileMapManager.h"

class EnemyAI {
public:
    
    EnemyAI(Player &player, TileMapManager &tileMapManager);

    void update(float dt);
    void updateBehavior(float deltaTime);

    sf::Vector2f getPosition() const;
    int getJumpCount() const;
    float getJumpTimer() const;
    bool isInAggressiveMode() const;

private:
    Player& player;
    TileMapManager& tileMapManager;
    std::unique_ptr<BehaviorTree> behaviorTree;

    int jumpCount = 0;
    float jumpTimer = 0.0f;
    const float jumpTimeLimit = 15.0f;  
    const int jumpThreshold = 10;
    float randTimeTrigger = 0.0f; 
    float randTimer = 0.0f;       

    void resetRandTimeTrigger();
    NodeStatus RandTimeTriggerElapsed();
    void initializeBehaviorTree();

    NodeStatus FastJumper();
    NodeStatus ChangePlayerPhysics();
    NodeStatus Wait(float);
    NodeStatus DeletePlatform();
    NodeStatus proximityThresholdReached();
    NodeStatus WarnPlayer();
    NodeStatus PlayerEscaped();
    NodeStatus AI_Retreats();
    NodeStatus AggressiveMode();
};

#endif //ENEMYAI_H