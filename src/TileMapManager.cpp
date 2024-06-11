#include "TileMapManager.h"
#include "Player.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <random>

namespace fs = std::filesystem;

TileMapManager::TileMapManager() : currentTileMap(nullptr), nextTileMap(nullptr), previousTileMap(nullptr), cameraX(0.0f),cameraY(0.0f) {
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
        tileMap->loadMap("resources/maps/" + info.filename);
        tileMaps[info.filename] = tileMap;
    }
}

std::vector<TileMap*> TileMapManager::getRenderedTileMaps() {
    std::vector<TileMap*> renderedTileMaps;
    if (currentTileMap) {
        renderedTileMaps.push_back(currentTileMap);
    }

    std::unordered_map<std::string, TileMap*> neighbours = getNeighbourTileMaps();
    for (const auto& neighbour : neighbours) {
        if (neighbour.second && std::find(renderedTileMaps.begin(), renderedTileMaps.end(), neighbour.second) == renderedTileMaps.end()) {
            renderedTileMaps.push_back(neighbour.second);
        }
    }

    return renderedTileMaps;
}


std::unordered_map<std::string, TileMap*> TileMapManager::getNeighbourTileMaps() {
    std::unordered_map<std::string, TileMap*> neighbours;
    if (currentTileMap == nullptr) {
        std::cerr << "Current tile map is nullptr" << std::endl;
        return neighbours;
    }

    sf::Vector2f currentPos = currentTileMap->getPosition();
    float tileSize = currentTileMap->getTileSize();

    for (const auto& tileMapInfo : tileMapOrder) {
        if (tileMaps.find(tileMapInfo.filename) != tileMaps.end()) {
            TileMap* tileMap = tileMaps[tileMapInfo.filename];
            sf::Vector2f pos = tileMap->getPosition();

            if (pos.x == currentPos.x && pos.y == currentPos.y - currentTileMap->getHeight() * tileSize) {
                neighbours["up"] = tileMap;
            } else if (pos.x == currentPos.x && pos.y == currentPos.y + currentTileMap->getHeight() * tileSize) {
                neighbours["down"] = tileMap;
            } else if (pos.x == currentPos.x - currentTileMap->getWidth() * tileSize && pos.y == currentPos.y) {
                neighbours["left"] = tileMap;
            } else if (pos.x == currentPos.x + currentTileMap->getWidth() * tileSize && pos.y == currentPos.y) {
                neighbours["right"] = tileMap;
            }
        }
    }

    return neighbours;
}

void TileMapManager::update(float deltaTime, Player *player, sf::RenderTarget &window) {
    if (!currentTileMap) {
        std::cerr << "No current tile map to update" << std::endl;
        return;
    }

    float playerX = player->getPosition().x;
    float playerY = player->getPosition().y;
    float playerTilePositionX = playerX / currentTileMap->getTileSize();
    float playerTilePositionY = playerY / currentTileMap->getTileSize();

    std::unordered_map<std::string, TileMap*> neighbours = getNeighbourTileMaps();

    int nextMapX = currentTileMap->getWidth() + (currentTileMap->getPosition().x / currentTileMap->getTileSize());
    int nextMapY = 2 + currentTileMap->getHeight() + (currentTileMap->getPosition().y / currentTileMap->getTileSize());
    int previousMapX = 3*(currentTileMap->getPosition().x / currentTileMap->getTileSize())/4;
    int previousMapY = 3*(currentTileMap->getPosition().y / currentTileMap->getTileSize())/4;
    if (playerTilePositionX >= nextMapX - 1 && neighbours.find("right") != neighbours.end()) {
        previousTileMap = currentTileMap;
        currentTileMap = neighbours["right"];
    } else if (playerTilePositionX <= previousMapX && neighbours.find("left") != neighbours.end()) {
        previousTileMap = currentTileMap;
        currentTileMap = neighbours["left"];
    } else if (playerTilePositionY >= nextMapY - 1 && neighbours.find("down") != neighbours.end()) {
        previousTileMap = currentTileMap;
        currentTileMap = neighbours["down"];
    } else if (playerTilePositionY <= previousMapY && neighbours.find("up") != neighbours.end()) {
        previousTileMap = currentTileMap;
        currentTileMap = neighbours["up"];
    }

    float relativePlayerX = playerX - currentTileMap->getPosition().x;
    float relativePlayerY = playerY - currentTileMap->getPosition().y;

    float targetCameraX = cameraX;
    float targetCameraY = cameraY;
    bool isTransitioningX = false;
    bool isTransitioningY = false;
    if ((neighbours.find("right") != neighbours.end() && (relativePlayerX >= (3 * currentTileMap->getWidth() * currentTileMap->getTileSize()) / 4)) || 
        (neighbours.find("left") != neighbours.end() && (relativePlayerX <= (1 * currentTileMap->getWidth() * currentTileMap->getTileSize()) / 4))) {
        isTransitioningX = true;
        targetCameraX = playerX - (window.getSize().x / 2);
    }

    if ((neighbours.find("down") != neighbours.end() && (relativePlayerY >= (3 * currentTileMap->getHeight() * currentTileMap->getTileSize()) / 4)) || 
        (neighbours.find("up") != neighbours.end() && (relativePlayerY <= (1 * currentTileMap->getHeight() * currentTileMap->getTileSize()) / 4))) { 
        isTransitioningY = true;
        targetCameraY = playerY - (window.getSize().y / 2);
    }
    float cameraSpeed = 0;
    if(isTransitioningX || isTransitioningY) {
        cameraSpeed = 300.0f;
    }
    if (!isTransitioningX) {
        targetCameraX = currentTileMap->getPosition().x;
    }
    if (!isTransitioningY) {
        targetCameraY = currentTileMap->getPosition().y;
    }
    if(!isTransitioningX && !isTransitioningY){
        cameraSpeed = 400.0f;
    }

    if (cameraX <= targetCameraX) {
        cameraX += cameraSpeed * deltaTime;
        if (cameraX > targetCameraX) {
            cameraX = targetCameraX;
        }
    } else if (cameraX > targetCameraX) {
        cameraX -= cameraSpeed * deltaTime;
        if (cameraX < targetCameraX) {
            cameraX = targetCameraX;
        }
    }
    
    if (cameraY <= targetCameraY) {
        cameraY += cameraSpeed * deltaTime;
        if (cameraY > targetCameraY) {
            cameraY = targetCameraY;
        }
    } else if (cameraY > targetCameraY) {
        cameraY -= cameraSpeed * deltaTime;
        if (cameraY < targetCameraY) {
            cameraY = targetCameraY;
        }
    }
}

void TileMapManager::render(sf::RenderTarget &target, bool debug) {
    sf::View cameraView(sf::FloatRect(cameraX, cameraY, target.getSize().x, target.getSize().y));
    target.setView(cameraView);

    currentTileMap->render(target, debug);

    std::unordered_map<std::string, TileMap*> neighbours = getNeighbourTileMaps();
    for (const auto& pair : neighbours) {
        pair.second->render(target, debug);
    }

    if (previousTileMap && previousTileMap != currentTileMap) {
        previousTileMap->render(target, debug);
    }
}

void TileMapManager::generateTileMapOrder(const std::string &directory, const std::string &outputFile, int tileWidth, int tileHeight) {
        std::vector<std::string> mapFiles;

    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".txt" && entry.path().filename() != "scores.txt") {
            mapFiles.push_back(entry.path().filename().string());
        }
    }

    if (mapFiles.empty()) {
        std::cerr << "No map files found in directory: " << directory << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(mapFiles.begin() + 1, mapFiles.end(), g); 

    std::vector<std::tuple<std::string, int, int>> orderedMaps;
    orderedMaps.emplace_back(mapFiles[0], 0, 0); //TODO: Replace first map with map.txt file

    int x = 0, y = 0;
    int dx = 0, dy = -1; 
    int steps = 1; 
    int steps_taken = 0;
    int segment_length = 1;
    int segment_passed = 0;
    int direction_changes = 0; 

    for (size_t i = 1; i < mapFiles.size(); ++i) {
        if (direction_changes % 2 == 0) {
            steps = segment_length;
        }

        if (steps_taken == steps) {
            if (dx == 0 && dy == -1) { // Up to right
                dx = 1; dy = 0;
            } else if (dx == 1 && dy == 0) { // Right to down
                dx = 0; dy = 1;
            } else if (dx == 0 && dy == 1) { // Down to left
                dx = -1; dy = 0;
            } else if (dx == -1 && dy == 0) { // Left to up
                dx = 0; dy = -1;
            }

            steps_taken = 0;
            direction_changes++;
            if (direction_changes % 2 == 0) {
                segment_length++;
            }
        }

        x += dx * tileWidth;
        y += dy * tileHeight;
        orderedMaps.emplace_back(mapFiles[i], x, y);
        steps_taken++;
    }

    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        return;
    }

    for (const auto &[filename, posX, posY] : orderedMaps) {
        outFile << filename << " " << posX << " " << posY << "\n";
    }

    outFile.close();
    std::cout << "Tile map order saved to " << outputFile << std::endl;
}
//NOTE: Not working with tileMaps with the same name
//TODO: Generer les maps avec une seed - (La 1ere doit tjrs etre plate et va rester dans resources) - Charger les maps avec le Manager (Les maps doivent etre supprimées a la fin)