#include "BehaviorTree.h"

using namespace std;

BehaviorTree::BehaviorTree(unique_ptr<Node> rootNode)
    : rootNode(move(rootNode)) {}

void BehaviorTree::setRoot(unique_ptr<Node> root){
    rootNode = move(root);
}

void BehaviorTree::run(){
    if (rootNode){
        rootNode->tick();
    }
}
