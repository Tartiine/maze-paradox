#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "TaskNode.h"

bool fastJumper = false;
bool randTimeTriggerElapsed = false;
bool proximityThresholdReached = false;
bool playerEscaped = false;
bool physicsChangeCooldown = true;

bool FastJumper() {
    if (fastJumper) {
        std::cout << "FastJumper: Player jumped quickly." << std::endl;
        return true;
    }
    return false;
}


bool RandTimeTriggerElapsed() {
    if (randTimeTriggerElapsed) {
        std::cout << "Random Time Trigger activated by player." << std::endl;
        return true;
    }
    return false;
}

bool ProximityThresholdReached() {
    if (proximityThresholdReached) {
        std::cout << "Player is near AI." << std::endl;
        return true;
    }
    return false;
}

bool PlayerEscaped() {
    if (playerEscaped) {
        std::cout << "Player escaped AI." << std::endl;
        return true;
    }
    return false;
}

bool PhysicsChangeCooldown() {
    if (physicsChangeCooldown) {
        std::cout << "Physics change cooldown finished." << std::endl;
        return true;
    }
    return false;
}

bool DeletePlatform() {
    std::cout << "AI: Deleting platform!" << std::endl;
    return true;
}

bool AIWarnsPlayer() {
    std::cout << "AI: Warns player to back off!" << std::endl;
    return true;
}

bool AI_Retreats() {
    std::cout << "AI: Retreating!" << std::endl;
    return true;
}

bool AggressiveMode() {
    std::cout << "AI: Entering aggressive mode." << std::endl;
    return true;
}

bool ChangePlayerPhysics() {
    std::cout << "AI: Changing player physics." << std::endl;
    return true;
}

bool Wait() {
    std::cout << "AI: Waiting passively." << std::endl;
    return true;
}

void modifyBooleanState(const std::string& booleanName, bool value) {
    if (booleanName == "FastJumper") {
        fastJumper = value;
    } else if (booleanName == "RandTimeTriggerElapsed") {
        randTimeTriggerElapsed = value;
    } else if (booleanName == "ProximityThresholdReached") {
        proximityThresholdReached = value;
    } else if (booleanName == "PlayerEscaped") {
        playerEscaped = value;
    } else if (booleanName == "PhysicsChangeCooldown") {
        physicsChangeCooldown = value;
    } else {
        std::cout << "Unknown boolean: " << booleanName << std::endl;
    }
}

void executePlayerTask(const std::string& taskName, std::shared_ptr<SelectorNode> root) {
    bool taskExecuted = false;

    if (taskName == "FastJumper") {
        taskExecuted = FastJumper();
    } else if (taskName == "RandTimeTriggerElapsed") {
        taskExecuted = RandTimeTriggerElapsed();
    } else if (taskName == "ProximityThresholdReached") {
        taskExecuted = ProximityThresholdReached();
    } else if (taskName == "PhysicsChangeCooldown") {
        taskExecuted = PhysicsChangeCooldown();
    } else if (taskName == "PlayerEscaped") {
        taskExecuted = PlayerEscaped();
    } else {
        std::cout << "Unknown or non-player task: " << taskName << std::endl;
    }

    if (taskExecuted) {
        std::cout << "Task executed successfully.\n";
    }
}

std::shared_ptr<SelectorNode> buildBehaviorTree() {
    auto root = std::make_shared<SelectorNode>();

    auto fastJumperSequence = std::make_shared<SequenceNode>();
    fastJumperSequence->addChild(std::make_shared<TaskNode>("FastJumper", FastJumper));
    fastJumperSequence->addChild(std::make_shared<TaskNode>("ChangePlayerPhysics", ChangePlayerPhysics));
    root->addChild(fastJumperSequence);

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
    root->addChild(std::make_shared<TaskNode>("Wait", Wait));

    return root;
}

int main() {
    auto root = buildBehaviorTree();

    std::string input;
    std::cout << "Enter commands to modify booleans or execute tree (type 'exit' to quit):" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        } else if (input == "execute tree") {
            std::cout << "Evaluating Behavior Tree..." << std::endl;
            root->tick();  
        } else if (input.rfind("set ", 0) == 0) {  
            size_t spacePos = input.find(' ', 4);
            if (spacePos != std::string::npos) {
                std::string booleanName = input.substr(4, spacePos - 4);
                bool value = input.substr(spacePos + 1) == "true";
                modifyBooleanState(booleanName, value);
                std::cout << "Set " << booleanName << " to " << (value ? "true" : "false") << ".\n";
            } else {
                std::cout << "Invalid 'set' command. Use: set <BooleanName> <true|false>\n";
            }
        } else {
            executePlayerTask(input, root); 
        }
    }

    std::cout << "Exiting behavior tree simulation." << std::endl;
    return 0;
}
