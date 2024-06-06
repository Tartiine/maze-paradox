#ifndef TILEMAP_MANAGER_H
#define TILEMAP_MANAGER_H

#include "TileMap.h"
#include "Player.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class TileMapManager {
public:
    TileMapManager();
    ~TileMapManager();
    void loadTileMaps(const std::string &fileName);
    TileMap* getCurrentTileMap();
    TileMap* getNextTileMap();
    void setCurrentTileMap(TileMap* tileMap);
    void update(float deltaTime, Player* player);
    void render(sf::RenderTarget &target, bool debug);

private:
    std::unordered_map<std::string, TileMap*> tileMaps;
    std::vector<std::string> tileMapOrder;
    TileMap* currentTileMap;
    float playerTilePosition;
};

#endif // TILEMAP_MANAGER_H
