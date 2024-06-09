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
        std::cout << "Parsed: " << mapFile << " " << x << " " << y << std::endl;
        tileMapOrder.push_back(TileMapInfo(mapFile, sf::Vector2f(x, y)));
    }

    inFile.close();

    if (!tileMapOrder.empty()) {
        for (const auto& tileMapInfo : tileMapOrder) {
            loadTileMap(tileMapInfo);
        }
        setCurrentTileMap(tileMaps[tileMapOrder.front().filename]);
    }
}

void TileMapManager::loadTileMap(const TileMapInfo &info) { 
    if (tileMaps.find(info.filename) == tileMaps.end()) {
        TileMap* tileMap = new TileMap(16, 12, 50.0f, info.filename);//FIXME: will there be a problem if two maps have the same name ???
        tileMap->setPosition(info.position);
        tileMap->loadMap("resources/" + info.filename);
        tileMaps[info.filename] = tileMap;
    }
}

TileMap* TileMapManager::getCurrentTileMap() {
    return currentTileMap;
}

TileMap* TileMapManager::getNextTileMap() {
    auto it = std::find_if(tileMapOrder.begin(), tileMapOrder.end(), 
                        [this](const TileMapInfo& info) { return info.filename == currentTileMap->getName(); });
    if (it != tileMapOrder.end()) {
        int nextIndex = (std::distance(tileMapOrder.begin(), it) + 1) % tileMapOrder.size();
        return tileMaps[tileMapOrder[nextIndex].filename];
    }
    return nullptr;
}
//TODO: update and render fonction -> camera follows the player only if he's in the last quarter of a tilemap, else the camera is fixed

void TileMapManager::update(float deltaTime, Player *player) {
    setPlayerX(player->getPosition().x);
    playerTilePosition = playerX / currentTileMap->getTileSize();
    std::cout << "playerTilePosition: " << playerTilePosition << std::endl;

    nextTileMap = getNextTileMap();
    if (nextTileMap != nullptr) {
        std::cout << "nextTileMap is not nullptr" << std::endl;
    } else {
        std::cout << "nextTileMap is nullptr" << std::endl;
    }

    int endMap = (2 * currentTileMap->getWidth() + (currentTileMap->getPosition().x/currentTileMap->getTileSize())) / 3;
    int nextMap = currentTileMap->getWidth() + (currentTileMap->getPosition().x/currentTileMap->getTileSize());
    std::cout << "endMap: " << endMap << " nextMap: " << nextMap << std::endl;
    if (playerTilePosition >= nextMap-1) {
        std::cout << "NEXT MAP " << std::endl;
        if (nextTileMap != nullptr) {
            setCurrentTileMap(nextTileMap);
        }
    }
}

void TileMapManager::render(sf::RenderTarget &target, bool debug) {
    if (playerTilePosition >= (3 * currentTileMap->getWidth() + (currentTileMap->getPosition().x/currentTileMap->getTileSize())) / 4) {
        for(int i = 0; cameraX < playerX - (target.getSize().x / 2); i++) {
            cameraX += 0.00001f;
            sf::View cameraView(sf::FloatRect(cameraX, 0, target.getSize().x, target.getSize().y));
            target.setView(cameraView);
        }
    } else {
        cameraX = currentTileMap->getPosition().x + (currentTileMap->getWidth() * currentTileMap->getTileSize()) / 2 - (target.getSize().x / 2);
            sf::View cameraView(sf::FloatRect(cameraX, 0, target.getSize().x, target.getSize().y));
            target.setView(cameraView);
    }

    currentTileMap->render(target, debug);
    nextTileMap->render(target, debug);
}

//FIXME: smoother transition 
//TODO: scroll in every direction

float TileMapManager::getPlayerX() const {
    return playerX;
}

void TileMapManager::setCurrentTileMap(TileMap *tileMap) {
    currentTileMap = tileMap;
}

void TileMapManager::setPlayerX(float playerX) {
    this->playerX = playerX;
}
