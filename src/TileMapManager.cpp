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

void TileMapManager::loadTileMaps(const string &fileName) {
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        cerr << "Failed to open tile map order file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        istringstream ss(line);
        string mapFile;
        float x, y;
        if (!(ss >> mapFile >> x >> y)) {
            cerr << "Failed to parse line: \"" << line << "\" in tile map order file: " << fileName << endl; //TODO: Skip empty lines
            continue;
        }
        tileMapOrder.push_back(TileMapInfo(mapFile, sf::Vector2f(x, y)));
    }

    inFile.close();

    if (!tileMapOrder.empty()) {
        for (const auto& tileMapInfo : tileMapOrder) {
            loadTileMap(tileMapInfo);
        }
        currentTileMap = tileMaps[tileMapOrder.front().filename].get();
    }
}

void TileMapManager::loadTileMap(const TileMapInfo &info) { 
    if (tileMaps.find(info.filename) == tileMaps.end()) {
        auto tileMap = make_unique<TileMap>(40, 22, 16.f, info.filename);
        tileMap->setPosition(info.position);
        tileMap->loadMap("resources/" + info.filename);
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

    unordered_map<string, TileMap*> neighbours = getNeighbourTileMaps();
    for (const auto& pair : neighbours) {
        pair.second->render(target, debug);
    }
    

    if (previousTileMap && previousTileMap != currentTileMap) {
        previousTileMap->render(target, debug);
    }
}

void TileMapManager::generateTileMapOrder(const vector<string> &directories, const string &outputFile, int tileWidth, int tileHeight) {
vector<string> mapFiles;


    for (const auto &directory : directories) {
        for (const auto &entry : fs::directory_iterator(directory)) {
            if (entry.path().extension() == ".txt" && entry.path().filename() != "scores.txt") {
                mapFiles.push_back(entry.path().string());
            }
        }
    }

    if (mapFiles.empty()) {
        cerr << "No map files found in the provided directories" << endl;
        return;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(mapFiles.begin(), mapFiles.end(), g);

    vector<tuple<string, int, int>> orderedMaps;
    bool mapTxtFound = false;

    string resourcesFolder = "resources";
    try {
        for (const auto &entry : fs::directory_iterator(resourcesFolder)) {
            if (entry.is_regular_file() && entry.path().filename() == "map.txt") {
                string relativePath = fs::relative(entry.path(), resourcesFolder).string();
                orderedMaps.emplace_back(relativePath, 0, 0);
                mapTxtFound = true;
                break;
            }
        }
    } catch (const fs::filesystem_error &e) {
        cerr << "Filesystem error: " << e.what() << endl;
    }

    if (!mapTxtFound) {
        string relativePath = fs::relative(mapFiles[0], "resources").string();
        orderedMaps.emplace_back(relativePath, 0, 0);
        mapFiles.erase(mapFiles.begin());
    }

    int x = 0, y = 0;
    int dx = 0, dy = -1; 
    int steps = 1; 
    int steps_taken = 0;
    int segment_length = 1;
    int segment_passed = 0;
    int direction_changes = 0; 

    for (size_t i = 0; i < mapFiles.size(); ++i) {
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
        string relativePath = fs::relative(mapFiles[i], "resources").string();
        orderedMaps.emplace_back(relativePath, x, y);
        steps_taken++;
    }

    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Failed to open output file: " << outputFile << endl;
        return;
    }

    for (const auto &[filename, posX, posY] : orderedMaps) {
        outFile << filename << " " << posX << " " << posY << "\n";
    }

    outFile.close();
    cout << "Tile map order saved to " << outputFile << endl;
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
    string chosenMapFile = tileMapOrder[randomIndex].filename;

    TileMap* chosenMap = tileMaps[chosenMapFile].get();
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

        if (chosenMap->getTile(x, y) == 0 &&
            chosenMap->getTile(x + 1, y) == 0 &&
            chosenMap->getTile(x, y + 1) == 0 &&
            chosenMap->getTile(x + 1, y + 1) == 0) {
            tileX = x+1;
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
    portalSprite.setTextureRect(sf::IntRect(0,0,32,32));
    portalSprite.setPosition(static_cast<float>(tileX * chosenMap->getTileSize() + chosenMap->getPosition().x),
                             static_cast<float>(tileY * chosenMap->getTileSize() + chosenMap->getPosition().y));
    //cout << "Portal placed at (" << tileX << ", " << tileY << ") in map " << chosenMapFile << endl;

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
        cout << "Player has touched the portal." << endl;
        return true;
    }
    return false;
}

//FIXME: Move and clean portal logic