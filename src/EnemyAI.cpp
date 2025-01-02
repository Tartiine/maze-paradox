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

    auto timeTriggeredSequence = make_shared<SequenceNode>();
    timeTriggeredSequence->addChild(make_shared<TaskNode>("RandTimeTriggerElapsed", [this]() { return RandTimeTriggerElapsed(); }));
    timeTriggeredSequence->addChild(make_shared<TaskNode>("DeletePlatform", [this]() { return DeletePlatform(); }));
    root->addChild(timeTriggeredSequence);

    auto proximitySequence = make_shared<SequenceNode>();
    auto proximitySelector = make_shared<SelectorNode>();

    proximitySequence->addChild(make_shared<TaskNode>("ProximityThresholdReached", [this]() { return proximityThresholdReached(); }));
    proximitySequence->addChild(make_shared<TaskNode>("AIWarnsPlayer", [this]() { return WarnPlayer(); }));

    auto playerEscapeSequence = make_shared<SequenceNode>();
    playerEscapeSequence->addChild(make_shared<TaskNode>("PlayerEscaped", [this]() { return PlayerEscaped(); }));
    playerEscapeSequence->addChild(make_shared<TaskNode>("AI Retreats", [this]() { return AI_Retreats(); }));

    proximitySelector->addChild(playerEscapeSequence);
    proximitySelector->addChild(make_shared<TaskNode>("AggressiveMode", [this]() { return AggressiveMode(); }));

    proximitySequence->addChild(proximitySelector);

    root->addChild(proximitySequence);

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
        cout << "FastJumper Condition Met!\n";
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
    cout << "Waiting" << endl;
    waitTimer += 0.1f;

    if (waitTimer >= duration) {
        waitTimer = 0.0f; 
        return NodeStatus::Success;
    }

    return NodeStatus::Running;
}

NodeStatus EnemyAI::RandTimeTriggerElapsed() {  
    randTimer += 0.01f;

    if (randTimer >= randTimeTrigger) {
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

NodeStatus EnemyAI::proximityThresholdReached() {
    static bool fakeThreshold = false; 
    fakeThreshold = !fakeThreshold; 

    if (fakeThreshold) {
        cout << "Mock: Proximity threshold reached (success).\n";
        return NodeStatus::Success;
    } else {
        cout << "Mock: Proximity threshold not reached (failure).\n";
        return NodeStatus::Failure;
    }

    /*
    float distance = sqrt(pow(player.getPosition().x - position.x, 2) +
                          pow(player.getPosition().y - position.y, 2));

    const float proximityThreshold = 200.0f; 
    
    if (distance <= proximityThreshold) {
        cout << "Proximity threshold reached. Distance: " << distance << "\n";
        return NodeStatus::Success;
    }

    cout << "Player is outside proximity threshold. Distance: " << distance << "\n";
    return NodeStatus::Failure;
    */
}


NodeStatus EnemyAI::WarnPlayer() {
    cout << "AI warns the player: Stay back!\n";

    //Warning (sound, animation, visual effect)

    return NodeStatus::Success;
}



NodeStatus EnemyAI::PlayerEscaped() {
    static bool fakeEscapeStatus = false; 
    fakeEscapeStatus = !fakeEscapeStatus; 
    
    if (fakeEscapeStatus) {
        cout << "Player has escaped (mocked). Distance: " << "N/A (mocked)" << "\n";
        return NodeStatus::Success;
    }

    cout << "Player is still within range (mocked). Distance: " << "N/A (mocked)" << "\n";
    return NodeStatus::Failure;

    /*
    float distance = sqrt(pow(player.getPosition().x - position.x, 2) +
                          pow(player.getPosition().y - position.y, 2));
    const float escapeThreshold = 300.0f; // Example threshold in pixels
    
    if (distance > escapeThreshold) {
        cout << "Player has escaped. Distance: " << distance << "\n";
        return NodeStatus::Success;
    }

    cout << "Player is still within range. Distance: " << distance << "\n";
    return NodeStatus::Failure;
    */
}

NodeStatus EnemyAI::AI_Retreats() {
    static bool fakeRetreatStatus = false; 
    fakeRetreatStatus = !fakeRetreatStatus; 

    if (fakeRetreatStatus) {
        cout << "AI retreating (mocked).\n";
        return NodeStatus::Running;
    } else {
        cout << "AI has successfully retreated (mocked).\n";
        return NodeStatus::Success;
    }

    /*
    sf::Vector2f retreatPoint = {initialPosition.x, initialPosition.y};

    sf::Vector2f direction = retreatPoint - position;
    float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (magnitude > 0.0f) {
        direction /= magnitude; // Normalize the direction vector
        position += direction * retreatSpeed; // retreatSpeed is a predefined constant
    }

    cout << "AI retreating to position: (" << retreatPoint.x << ", " << retreatPoint.y << ")\n";

    if (magnitude <= 10.0f) { // 10 is a small threshold
        cout << "AI has successfully retreated.\n";
        return NodeStatus::Success;
    }

    return NodeStatus::Running;
    */
}

NodeStatus EnemyAI::AggressiveMode() {
    cout << "AI enters aggressive mode!\n";

    //enterAggressiveState(); 

    return NodeStatus::Success;
}

//TODO: indicate to the player where the portal is located