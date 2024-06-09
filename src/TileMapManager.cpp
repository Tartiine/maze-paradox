#include "TileMapManager.h"
#include "Player.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

TileMapManager::TileMapManager() : currentTileMap(nullptr), nextTileMap(nullptr), previousTileMap(nullptr), cameraX(0.0f) {
}

TileMapManager::~TileMapManager() {
    for (auto &pair : tileMaps) {
        delete pair.second;
    }
    tileMaps.clear();
}

void TileMapManager::loadTileMaps(const std::string &fileName) {
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open tile map order file: " << fileName << std::endl;
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream ss(line);
        std::string mapFile;
        float x, y;
        if (!(ss >> mapFile >> x >> y)) {
            std::cerr << "Failed to parse line: \"" << line << "\" in tile map order file: " << fileName << std::endl;
            continue;
        }
        tileMapOrder.push_back(TileMapInfo(mapFile, sf::Vector2f(x, y)));
    }

    inFile.close();

    if (!tileMapOrder.empty()) {
        for (const auto& tileMapInfo : tileMapOrder) {
            loadTileMap(tileMapInfo);
        }
        currentTileMap = tileMaps[tileMapOrder.front().filename];
    }
}

void TileMapManager::loadTileMap(const TileMapInfo &info) { 
    if (tileMaps.find(info.filename) == tileMaps.end()) {
        TileMap* tileMap = new TileMap(16, 12, 50.0f, info.filename);
        tileMap->setPosition(info.position);
        tileMap->loadMap("resources/" + info.filename);
        tileMaps[info.filename] = tileMap;
    }
}

std::vector<TileMap*> TileMapManager::getRenderedTileMaps() const {
    std::vector<TileMap*> renderedTileMaps;
    if (currentTileMap) {
        renderedTileMaps.push_back(currentTileMap);
    }
    if (previousTileMap && previousTileMap != currentTileMap) { 
        renderedTileMaps.push_back(previousTileMap);
    }
    return renderedTileMaps;
}


TileMap* TileMapManager::getNextTileMap() { 
    auto it = std::find_if(tileMapOrder.begin(), tileMapOrder.end(), 
                        [this](const TileMapInfo& info) { return info.filename == currentTileMap->getName(); });
    if (it != tileMapOrder.end()) { 
        int currentIndex = std::distance(tileMapOrder.begin(), it);
        int nextIndex = currentIndex + 1;
        if (nextIndex < tileMapOrder.size()) {
            return tileMaps[tileMapOrder[nextIndex].filename];
        } else {
            return nullptr; 
        }
    }
    return nullptr;
}

//TODO: implement in every direction
void TileMapManager::update(float deltaTime, Player *player,sf::RenderTarget &window) {
    float playerX = player->getPosition().x;
    float playerTilePosition = playerX / currentTileMap->getTileSize();
    nextTileMap = getNextTileMap();
    int nextMap = currentTileMap->getWidth() + (currentTileMap->getPosition().x / currentTileMap->getTileSize());

    if (playerTilePosition >= nextMap - 1) {
        if (nextTileMap) {
            previousTileMap = currentTileMap;
            currentTileMap = nextTileMap;
        }
    }

    float relativePlayerX = playerX - currentTileMap->getPosition().x;
    if (nextTileMap && (relativePlayerX >= (3 * currentTileMap->getWidth() * currentTileMap->getTileSize()) / 4 && nextTileMap) || ((relativePlayerX <= (1 * currentTileMap->getWidth() * currentTileMap->getTileSize()) / 4 ) && previousTileMap)) {
        float targetCameraX = playerX - (window.getSize().x / 2);
        if (cameraX < targetCameraX) {
            cameraX += 300.0f * deltaTime;
            if (cameraX > targetCameraX) {
                cameraX = targetCameraX;
            }
        }
    } else {
        float targetCameraX = currentTileMap->getPosition().x + (currentTileMap->getWidth() * currentTileMap->getTileSize()) / 2 - (window.getSize().x / 2);
        if (cameraX < targetCameraX) {
            cameraX += 400.0f * deltaTime;
            if (cameraX > targetCameraX) {
                cameraX = targetCameraX;
            }
        }
    }

}

void TileMapManager::render(sf::RenderTarget &target, bool debug) {
    sf::View cameraView(sf::FloatRect(cameraX, 0, target.getSize().x, target.getSize().y));
    target.setView(cameraView);

    currentTileMap->render(target, debug);
    if (nextTileMap) {
        nextTileMap->render(target, debug);
    }
    if (previousTileMap) {
        previousTileMap->render(target, debug);
    }
}


