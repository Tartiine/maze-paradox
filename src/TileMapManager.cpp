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

using namespace std;

namespace fs = filesystem;

TileMapManager::TileMapManager() : currentTileMap(nullptr), nextTileMap(nullptr), previousTileMap(nullptr), cameraX(0.0f), cameraY(0.0f){
}

TileMapManager::~TileMapManager() {
    tileMaps.clear();
}

void TileMapManager::loadTileMaps(const vector<vector<vector<uint8_t>>>& tileMapBatches) {
    tileMaps.clear();

    for (const auto& info : tileMapOrder) {
        size_t batchIdx = 0, mapIdx = 0;
        sscanf(info.filename.c_str(), "TileMap_%zu_%zu", &batchIdx, &mapIdx);

        if (batchIdx < tileMapBatches.size() && mapIdx < tileMapBatches[batchIdx].size()) {
            loadTileMap(info, tileMapBatches[batchIdx][mapIdx]);
        } else {
            cerr << "Invalid batchIdx or mapIdx: " << batchIdx << ", " << mapIdx << " from " << info.filename << endl;
        }
    }

    if (!tileMapOrder.empty()) {
        currentTileMap = tileMaps[tileMapOrder.front().filename].get();
        if (currentTileMap) {
            cout << "Set currentTileMap to: " << tileMapOrder.front().filename << endl;
        } else {
            cerr << "Failed to set currentTileMap; map not found or is nullptr." << endl;
        }
    } else {
        cerr << "tileMapOrder is empty; no maps to set as currentTileMap." << endl;
    }
}


void TileMapManager::loadTileMap(const TileMapInfo &info, const vector<uint8_t>& binaryData) { 
    if (tileMaps.find(info.filename) == tileMaps.end()) {
        auto tileMap = make_unique<TileMap>(40, 22, 16.f, info.filename);
        tileMap->setPosition(info.position);
        tileMap->loadMapFromMemory(binaryData); 
        tileMaps[info.filename] = move(tileMap);
    }
}


vector<TileMap*> TileMapManager::getRenderedTileMaps() {
    vector<TileMap*> renderedTileMaps;
    if (currentTileMap) {
        renderedTileMaps.push_back(currentTileMap);
    }

    auto neighbours = getNeighbourTileMaps();
    for (auto& neighbour : neighbours) {
        if (neighbour.second && find(renderedTileMaps.begin(), renderedTileMaps.end(), neighbour.second) == renderedTileMaps.end()) {
            renderedTileMaps.push_back(neighbour.second);
        }
    }

    return renderedTileMaps;
}

unordered_map<string, TileMap*> TileMapManager::getNeighbourTileMaps() {
    unordered_map<string, TileMap*> neighbours;
    if (currentTileMap == nullptr) {
        cerr << "Current tile map is nullptr" << endl;
        return neighbours;
    }

    sf::Vector2f currentPos = currentTileMap->getPosition();
    float tileSize = currentTileMap->getTileSize();

    for (const auto& tileMapInfo : tileMapOrder) {
        if (tileMaps.find(tileMapInfo.filename) != tileMaps.end()) {
            TileMap* tileMap = tileMaps[tileMapInfo.filename].get();
            sf::Vector2f pos = tileMap->getPosition();

            if (pos.x == currentPos.x && pos.y == currentPos.y - currentTileMap->getHeight() * tileSize) {
                neighbours["up"] = tileMap;
            } else if (pos.x == currentPos.x && pos.y == currentPos.y + currentTileMap->getHeight() * tileSize) {
                neighbours["down"] = tileMap;
            } else if (pos.x == currentPos.x - currentTileMap->getWidth() * tileSize && pos.y == currentPos.y) {
                neighbours["left"] = tileMap;
            } else if (pos.x == currentPos.x + currentTileMap->getWidth() * tileSize && pos.y == currentPos.y) {
                neighbours["right"] = tileMap;
            } else if (pos.x == currentPos.x + currentTileMap->getWidth() * tileSize && pos.y == currentPos.y - currentTileMap->getHeight() * tileSize) {
                neighbours["diagonal_right_up"] = tileMap;
            } else if (pos.x == currentPos.x + currentTileMap->getWidth() * tileSize && pos.y == currentPos.y + currentTileMap->getHeight() * tileSize) {
                neighbours["diagonal_right_down"] = tileMap;
            } else if (pos.x == currentPos.x - currentTileMap->getWidth() * tileSize && pos.y == currentPos.y - currentTileMap->getHeight() * tileSize) {
                neighbours["diagonal_left_up"] = tileMap;
            } else if (pos.x == currentPos.x - currentTileMap->getWidth() * tileSize && pos.y == currentPos.y + currentTileMap->getHeight() * tileSize) {
                neighbours["diagonal_left_down"] = tileMap;
            }
        } else {
            cerr << "TileMap not found for filename: " << tileMapInfo.filename << endl;
        }
    }

    return neighbours;
}

void TileMapManager::update(float deltaTime, Player* player, sf::RenderTarget &window) {
    if (!currentTileMap) {
        cerr << "No current tile map to update" << endl;
        return;
    }

    for (auto it = lightEffects.begin(); it != lightEffects.end(); ) {
        it->update(deltaTime);
        if (it->isExpired()) {
            it = lightEffects.erase(it);
        } else {
            ++it;
        }
    }

    if (pendingDeletion) {
        deletionTimer -= deltaTime;

        if (deletionTimer <= 0.0f) {
            currentTileMap->deleteTile(tileXToDelete, tileYToDelete);
            pendingDeletion = false; 
        }
    }

    float playerX = player->getPosition().x;
    float playerY = player->getPosition().y;
    float playerTilePositionX = playerX / currentTileMap->getTileSize();
    float playerTilePositionY = playerY / currentTileMap->getTileSize();

    auto neighbours = getNeighbourTileMaps();
    int nextMapX = currentTileMap->getWidth() + (currentTileMap->getPosition().x / currentTileMap->getTileSize());
    int nextMapY = currentTileMap->getHeight() + (currentTileMap->getPosition().y / currentTileMap->getTileSize());
    int previousMapX = (currentTileMap->getPosition().x / currentTileMap->getTileSize())-10;
    int previousMapY = 3*(currentTileMap->getPosition().y / currentTileMap->getTileSize())/4;
    if (playerTilePositionX >= nextMapX - 1 && neighbours.find("right") != neighbours.end()) {
        cerr << "Switching to right tile map" << endl;
        previousTileMap = currentTileMap;
        currentTileMap = neighbours["right"];
    } else if (playerTilePositionX <= previousMapX && neighbours.find("left") != neighbours.end()) {
        cerr << "Switching to left tile map" << endl;
        previousTileMap = currentTileMap;
        currentTileMap = neighbours["left"];
    } else if (playerTilePositionY >= nextMapY - 1 && neighbours.find("down") != neighbours.end()) {
        cerr << "Switching to down tile map" << endl;
        previousTileMap = currentTileMap;
        currentTileMap = neighbours["down"];
    } else if (playerTilePositionY <= previousMapY && neighbours.find("up") != neighbours.end()) {
        cerr << "Switching to up tile map" << endl;
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

    for (const auto& lightEffect : lightEffects) {
        lightEffect.render(target);
    }
    
    unordered_map<string, TileMap*> neighbours = getNeighbourTileMaps();
    for (const auto& pair : neighbours) {
        pair.second->render(target, debug);
    }
    

    if (previousTileMap && previousTileMap != currentTileMap) {
        previousTileMap->render(target, debug);
    }
}

void TileMapManager::generateTileMapOrder(const vector<vector<vector<uint8_t>>>& tileMapBatches, int tileWidth, int tileHeight) {
    tileMapOrder.clear();

    int x = 0, y = 0;
    int dx = 0, dy = -1;
    int steps = 1, steps_taken = 0;
    int segment_length = 1;
    int direction_changes = 0;

    string mapName = "TileMap_0_0";
    tileMapOrder.emplace_back(mapName, sf::Vector2f(x, y));

    if (direction_changes % 2 == 0) steps = segment_length;
    if (steps_taken == steps) {
        if (dx == 0 && dy == -1) { dx = 1; dy = 0; }
        else if (dx == 1 && dy == 0) { dx = 0; dy = 1; }
        else if (dx == 0 && dy == 1) { dx = -1; dy = 0; }
        else if (dx == -1 && dy == 0) { dx = 0; dy = -1; }

        steps_taken = 0;
        direction_changes++;
        if (direction_changes % 2 == 0) segment_length++;
    }
    x += dx * tileWidth;
    y += dy * tileHeight;
    steps_taken++;

    vector<TileMapInfo> shuffledMaps;
    for (size_t batchIdx = 1; batchIdx < tileMapBatches.size(); ++batchIdx) {
        for (size_t mapIdx = 0; mapIdx < tileMapBatches[batchIdx].size(); ++mapIdx) {
            mapName = "TileMap_" + to_string(batchIdx) + "_" + to_string(mapIdx);
            shuffledMaps.emplace_back(mapName, sf::Vector2f(0, 0));  
        }
    }

    default_random_engine rng(random_device{}());
    shuffle(shuffledMaps.begin(), shuffledMaps.end(), rng);

    for (auto& info : shuffledMaps) {
        info.position = sf::Vector2f(x, y);
        tileMapOrder.push_back(info);

        if (direction_changes % 2 == 0) steps = segment_length;
        if (steps_taken == steps) {
            if (dx == 0 && dy == -1) { dx = 1; dy = 0; }
            else if (dx == 1 && dy == 0) { dx = 0; dy = 1; }
            else if (dx == 0 && dy == 1) { dx = -1; dy = 0; }
            else if (dx == -1 && dy == 0) { dx = 0; dy = -1; }

            steps_taken = 0;
            direction_changes++;
            if (direction_changes % 2 == 0) segment_length++;
        }
        x += dx * tileWidth;
        y += dy * tileHeight;
        steps_taken++;
    }

    loadTileMaps(tileMapBatches);
}



//NOTE: Not working with tileMaps with the same name
//TODO: Generer les maps avec une seed - (Les maps doivent etre supprimées a la fin)
//TODO: Camera must be faster when falling

void TileMapManager::createFinalMap() {
    if (tileMapOrder.size() <= 1) {
        cerr << "Not enough tile maps to choose from." << endl;
        return;
    }

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> distr(1, tileMapOrder.size() - 1);
    int randomIndex = distr(gen);
    TileMap* chosenMap = tileMaps[tileMapOrder[randomIndex].filename].get();
    
    if (!chosenMap) {
        cerr << "Chosen tile map not loaded." << endl;
        return;
    }

    uniform_int_distribution<> xDistr(0, chosenMap->getWidth() - 4);
    uniform_int_distribution<> yDistr(0, chosenMap->getHeight() - 4);

    int tileX = -1, tileY = -1;
    bool spotFound = false;

    for (int i = 0; i < 100; ++i) {
        int x = xDistr(gen);
        int y = yDistr(gen);
        if (x >= 0 && y >= 0 &&
            x + 1 < chosenMap->getHeight() && y + 1 < chosenMap->getWidth() &&
            chosenMap->getTile(x, y) == 0 &&
            chosenMap->getTile(x + 1, y) == 0 &&
            chosenMap->getTile(x, y + 1) == 0 &&
            chosenMap->getTile(x + 1, y + 1) == 0) {
            tileX = x + 1;
            tileY = y;
            spotFound = true;
            break;
        }
    }

    if (!spotFound) {
        cerr << "Failed to find a suitable spot for the portal." << endl;
        return;
    }

    if (!portalTexture.loadFromFile("resources/sprites/BluePortal.png")) {
        cerr << "Failed to load portal texture." << endl;
        return;
    }

    portalSprite.setTexture(portalTexture);
    portalSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    portalSprite.setPosition(static_cast<float>(tileX * chosenMap->getTileSize() + chosenMap->getPosition().x),
                             static_cast<float>(tileY * chosenMap->getTileSize() + chosenMap->getPosition().y));

    portalAnimation = make_unique<Animation>(&portalTexture, sf::Vector2u(9, 1), 0.15f, sf::Vector2u(32, 32));
}

void TileMapManager::updateAnimation(float deltaTime) {
    if (portalAnimation) {
        portalAnimation->update(0, deltaTime); 
        portalSprite.setTextureRect(portalAnimation->uvRect);
    }
}

void TileMapManager::render(sf::RenderTarget& target) {
    target.draw(portalSprite);
}

bool TileMapManager::checkPortal(Player* player) {
    if (player->isColliding(portalSprite.getGlobalBounds())) {
        cerr << "Player has touched the portal." << endl;
        return true;
    }
    return false;
}

//FIXME: Move and clean portal logic

void TileMapManager::deletePlatform(sf::Vector2f playerPosition) {
    if (!currentTileMap) {
        cerr << "No current tile map to delete a platform from.\n";
        return;
    }

    sf::Vector2f mapPos = currentTileMap->getPosition();
    float tileSize = currentTileMap->getTileSize();
    unsigned mapWidth = currentTileMap->getWidth();
    unsigned mapHeight = currentTileMap->getHeight();

    int playerTileX = static_cast<int>((playerPosition.x - mapPos.x) / tileSize);
    int playerTileY = static_cast<int>((playerPosition.y - mapPos.y) / tileSize);

    if (playerTileX < 0 || playerTileY < 0 || playerTileX >= mapWidth || playerTileY >= mapHeight) {
        cerr << "Player tile out of bounds: (" << playerTileX << ", " << playerTileY << ")\n";
        return;
    }

    constexpr int radius = 3;
    vector<pair<int, int>> candidateTiles;

    for (int y = playerTileY - radius; y <= playerTileY + radius; ++y) {
        for (int x = playerTileX - radius; x <= playerTileX + radius; ++x) {
            if (x < 0 || y < 0 || x >= static_cast<int>(mapHeight) || y >= static_cast<int>(mapWidth)) {
                continue; // Skip out-of-bounds tiles
            }

            if (!currentTileMap->getTile(x, y)) {
                continue; // Skip empty tiles
            }

            sf::FloatRect tileBounds(
                mapPos.x + x * tileSize, mapPos.y + y * tileSize, tileSize, tileSize);

            if (portalSprite.getGlobalBounds().intersects(tileBounds)) {
                cerr << "Skipping tile at (" << x << ", " << y << ") - intersects portal.\n";
                continue;
            }

            candidateTiles.emplace_back(x, y);
        }
    }

    if (candidateTiles.empty()) {
        cerr << "No valid candidate tiles found near the player.\n";
        return;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distr(0, candidateTiles.size() - 1);

    auto [tileX, tileY] = candidateTiles[distr(gen)];
    pendingDeletion = true;
    deletionTimer = 0.7f; 
    tileXToDelete = tileX;
    tileYToDelete = tileY;

    sf::Vector2f tilePosition = currentTileMap->getPosition() + sf::Vector2f(tileY * tileSize + tileSize / 2, tileX * tileSize + tileSize / 2);
    lightEffects.emplace_back(tilePosition, tileSize / 2, 0.6f); 
}

sf::Vector2i TileMapManager::getTileToDelete() const {
    return sf::Vector2i(tileXToDelete, tileYToDelete);
}

//TODO: Move some logic to a cameraManager class
