#include "EnemyAI.h"
#include "BehaviorTree.h"
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "TaskNode.h"

void EnemyAI::initializeBehaviorTree() {

    auto root = std::make_shared<SelectorNode>();

    auto fastJumperSequence = std::make_shared<SequenceNode>();
    fastJumperSequence->addChild(std::make_shared<TaskNode>("FastJumper", FastJumper));
    fastJumperSequence->addChild(std::make_shared<TaskNode>("ChangePlayerPhysics", ChangePlayerPhysics));
    root->addChild(fastJumperSequence);
    /* 
    auto timeTriggeredSequence = std::make_shared<SequenceNode>();
    timeTriggeredSequence->addChild(std::make_shared<TaskNode>("RandTimeTriggerElapsed", RandTimeTriggerElapsed));
    timeTriggeredSequence->addChild(std::make_shared<TaskNode>("PhysicsChangeCooldown", PhysicsChangeCooldown));
    timeTriggeredSequence->addChild(std::make_shared<TaskNode>("DeletePlatform", DeletePlatform));
    root->addChild(timeTriggeredSequence);

    auto proximitySequence = std::make_shared<SequenceNode>();
    proximitySequence->addChild(std::make_shared<TaskNode>("ProximityThresholdReached", ProximityThresholdReached));
    proximitySequence->addChild(std::make_shared<TaskNode>("AI Warns Player", AIWarnsPlayer));

    auto proximitySelector = std::make_shared<SelectorNode>();
    auto playerEscapeSequence = std::make_shared<SequenceNode>();
    playerEscapeSequence->addChild(std::make_shared<TaskNode>("PlayerEscaped", PlayerEscaped));
    playerEscapeSequence->addChild(std::make_shared<TaskNode>("AI Retreats", AI_Retreats));
    proximitySelector->addChild(playerEscapeSequence);
    proximitySelector->addChild(std::make_shared<TaskNode>("AggressiveMode", AggressiveMode));
    proximitySequence->addChild(proximitySelector); 
    root->addChild(proximitySequence);
    */
    root->addChild(std::make_shared<TaskNode>("Wait", Wait));
    
    behaviorTree = std::make_unique<BehaviorTree>(std::move(root));
}

void EnemyAI::updateBehavior(float deltaTime) {
    behaviorTree->run();
}
