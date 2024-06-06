#include "TileMapManager.h"
#include "Player.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

TileMapManager::TileMapManager() : currentTileMap(nullptr), playerTilePosition(0.0f) {

}

TileMapManager::~TileMapManager() {
    for (auto &pair : tileMaps) {
        delete pair.second;
    }
    tileMaps.clear();
}

void TileMapManager::loadTileMaps(const std::string &filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open tile map order file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        tileMapOrder.push_back(line);
    }
    inFile.close();

    for (const auto &filename : tileMapOrder) {
        TileMap* tileMap = new TileMap(16, 12, 50.0f, filename);
        tileMap->loadMap("resources/" + filename + ".txt");
        tileMaps[filename] = tileMap;
    }

    currentTileMap = tileMaps[tileMapOrder[0]];
}

TileMap* TileMapManager::getCurrentTileMap() {
    return currentTileMap;
}

TileMap* TileMapManager::getNextTileMap() {
    auto it = std::find(tileMapOrder.begin(), tileMapOrder.end(), currentTileMap->getName());
    if (it != tileMapOrder.end()) {
        int nextIndex = (std::distance(tileMapOrder.begin(), it) + 1) % tileMapOrder.size();
        return tileMaps[tileMapOrder[nextIndex]];
    }
    return nullptr;
}

void TileMapManager::update(float deltaTime, Player *player) {
    float playerX = player->getPosition().x;  
    playerTilePosition = playerX / currentTileMap->getTileSize();
    if (playerTilePosition >= currentTileMap->getWidth()) {
        TileMap* nextTileMap = getNextTileMap();
        if (nextTileMap) {
            currentTileMap = nextTileMap;
            playerTilePosition = 0.0f;
        }
    }
}

void TileMapManager::render(sf::RenderTarget &target, bool debug) {
    float cameraX = playerTilePosition * currentTileMap->getTileSize() - (target.getSize().x / 2);
    if (cameraX < 0) {
        cameraX = 0;
    }
    if (cameraX >= (currentTileMap->getWidth() * currentTileMap->getTileSize())/2) { 
        std::cout << "cameraX: " << cameraX << " Target:" << target.getSize().x << std::endl;
        std::cout << " TileMapWidth: " << currentTileMap->getWidth() << " TileSize:" << currentTileMap->getTileSize() << std::endl;
        cameraX = currentTileMap->getWidth() * currentTileMap->getTileSize() - target.getSize().x;
    }

    sf::View cameraView(sf::FloatRect(cameraX, 0, target.getSize().x, target.getSize().y));
    target.setView(cameraView);

    currentTileMap->render(target, debug);
}

void TileMapManager::setCurrentTileMap(TileMap* tileMap) {
    currentTileMap = tileMap;
}