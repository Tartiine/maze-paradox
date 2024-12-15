#include "EnemyAI.h"

#include <chrono>  

using namespace std;


EnemyAI::EnemyAI(Player& player, TileMapManager& tileMapManager) : player(player), tileMapManager(tileMapManager) {
    initializeBehaviorTree();
}

void EnemyAI::initializeBehaviorTree() {

    auto root = make_unique<SelectorNode>();

    auto fastJumperSequence = make_unique<SequenceNode>();
    fastJumperSequence->addChild(make_unique<TaskNode>("FastJumper", [this]() { return this->FastJumper(); }));
    fastJumperSequence->addChild(make_unique<TaskNode>("ChangePlayerPhysics", [this]() { return this->ChangePlayerPhysics(); }));

    root->addChild(move(fastJumperSequence));

    auto timeTriggeredSequence = std::make_shared<SequenceNode>();
    timeTriggeredSequence->addChild(std::make_shared<TaskNode>("RandTimeTriggerElapsed", [this]() { return RandTimeTriggerElapsed(); }));
    timeTriggeredSequence->addChild(std::make_shared<TaskNode>("DeletePlatform", [this]() { return DeletePlatform(); }));
    root->addChild(timeTriggeredSequence);

    /* 
    auto timeTriggeredSequence = make_shared<SequenceNode>();
    timeTriggeredSequence->addChild(make_shared<TaskNode>("RandTimeTriggerElapsed", RandTimeTriggerElapsed));
    timeTriggeredSequence->addChild(make_shared<TaskNode>("PhysicsChangeCooldown", PhysicsChangeCooldown));
    timeTriggeredSequence->addChild(make_shared<TaskNode>("DeletePlatform", DeletePlatform));
    root->addChild(timeTriggeredSequence);

    auto proximitySequence = make_shared<SequenceNode>();
    proximitySequence->addChild(make_shared<TaskNode>("ProximityThresholdReached", ProximityThresholdReached));
    proximitySequence->addChild(make_shared<TaskNode>("AI Warns Player", AIWarnsPlayer));

    auto proximitySelector = make_shared<SelectorNode>();
    auto playerEscapeSequence = make_shared<SequenceNode>();
    playerEscapeSequence->addChild(make_shared<TaskNode>("PlayerEscaped", PlayerEscaped));
    playerEscapeSequence->addChild(make_shared<TaskNode>("AI Retreats", AI_Retreats));
    proximitySelector->addChild(playerEscapeSequence);
    proximitySelector->addChild(make_shared<TaskNode>("AggressiveMode", AggressiveMode));
    proximitySequence->addChild(proximitySelector); 
    root->addChild(proximitySequence);
    */

    root->addChild(make_unique<TaskNode>("Wait", [this]() { return this->Wait(1.0f); }));

    behaviorTree = make_unique<BehaviorTree>(move(root));

}

void EnemyAI::update(float deltaTime) {
    if (player.hasJumped()) {
        jumpCount++;
    }

    jumpTimer += deltaTime;

    if (jumpTimer > jumpTimeLimit) {
        jumpCount = 0;
        jumpTimer = 0.0f;
    }

    player.updatePhysicsEffect(deltaTime);
    updateBehavior(deltaTime);

}

void EnemyAI::updateBehavior(float deltaTime) {
    if (behaviorTree) {
        behaviorTree->run();
    }
}

NodeStatus EnemyAI::FastJumper() {

    if (jumpCount >= jumpThreshold && jumpTimer <= jumpTimeLimit) {
        std::cout << "FastJumper Condition Met!\n";
        jumpCount = 0;
        jumpTimer = 0.0f;
        return NodeStatus::Success;
    }

    return NodeStatus::Failure;
}

NodeStatus EnemyAI::ChangePlayerPhysics() {
    cout << "Changing Player Physics...\n";
    player.changePhysics(10.0f);
    return NodeStatus::Success;
}

NodeStatus EnemyAI::Wait(float duration) {
    static float waitTimer = 0.0f;

    waitTimer += 0.1f;

    if (waitTimer >= duration) {
        waitTimer = 0.0f; 
        return NodeStatus::Success;
    }

    return NodeStatus::Running;
}

NodeStatus EnemyAI::RandTimeTriggerElapsed() {  //TODO: Add minimum delay (3 seconds)
    randTimer += 0.01f;

    if (randTimer >= randTimeTrigger) {
        std::cout << "RandTimeTriggerElapsed: Trigger met after " << randTimeTrigger << " seconds\n";
        resetRandTimeTrigger(); 
        return NodeStatus::Success;
    }

    return NodeStatus::Running;
}

void EnemyAI::resetRandTimeTrigger() {
    randTimeTrigger = 3.0f + static_cast<float>(rand() % 5); 
    randTimer = 0.0f; 
}


NodeStatus EnemyAI::DeletePlatform() {
    cout << "Deleting platform...\n";
    sf::Vector2f playerPosition = player.getPosition();
    tileMapManager.deletePlatform(playerPosition);
    return NodeStatus::Success;
}
//TODO: Animation when deleting a platform
//TODO: indicate to the player where the portal is located