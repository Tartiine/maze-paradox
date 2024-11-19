#include "EnemyAI.h"


using namespace std;


EnemyAI::EnemyAI(Player& player) : player(player) {
    initializeBehaviorTree();
}

void EnemyAI::initializeBehaviorTree() {

    auto root = make_unique<SelectorNode>();

    auto fastJumperSequence = make_unique<SequenceNode>();
    fastJumperSequence->addChild(make_unique<TaskNode>("FastJumper", [this]() { return this->FastJumper(); }));
    fastJumperSequence->addChild(make_unique<TaskNode>("ChangePlayerPhysics", [this]() { return this->ChangePlayerPhysics(); }));

    root->addChild(move(fastJumperSequence));

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

    root->addChild(make_unique<TaskNode>("Wait", [this]() { return this->Wait(); }));

    behaviorTree = make_unique<BehaviorTree>(move(root));

}

void EnemyAI::update(float deltaTime) {
    jumpTimer += deltaTime; 

    behaviorTree->run();  
}

NodeStatus EnemyAI::FastJumper() {
    cout << "Checking FastJumper Condition...\n";

    if (player.hasJumped()) {
        jumpCount++;
        cout << "Jump Count: " << jumpCount << "\n";
    }

    if (jumpCount >= jumpThreshold && jumpTimer <= jumpTimeLimit) {
        cout << "FastJumper Condition Met!\n";
        return NodeStatus::Success;
    }

    if (jumpTimer > jumpTimeLimit) {
        jumpCount = 0;
        jumpTimer = 0.0f;
    }

    return NodeStatus::Failure;
}

NodeStatus EnemyAI::ChangePlayerPhysics() {
    cout << "Changing Player Physics...\n";
    player.changePhysics();
    return NodeStatus::Success;
}

NodeStatus EnemyAI::Wait() {
    return NodeStatus();
}
