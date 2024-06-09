#ifndef TILEMAP_MANAGER_H
#define TILEMAP_MANAGER_H

#include "TileMap.h"
#include "Player.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

struct TileMapInfo {
    std::string filename;
    sf::Vector2f position;

    TileMapInfo(const std::string& file, const sf::Vector2f& pos)
        : filename(file), position(pos) {}
};

class TileMapManager {
public:
    TileMapManager();
    ~TileMapManager();
    void loadTileMaps(const std::string &fileName);
    TileMap* getCurrentTileMap();
    TileMap* getNextTileMap();
    float getPlayerX() const;
    void setCurrentTileMap(TileMap* tileMap);
    void setPlayerX(float playerX);
    void update(float deltaTime, Player* player);
    void render(sf::RenderTarget &target, bool debug);

private:
    std::unordered_map<std::string, TileMap*> tileMaps;
    std::vector<TileMapInfo> tileMapOrder;
    TileMap* currentTileMap;
    TileMap* nextTileMap;
    TileMap* previousTileMap;
    float playerTilePosition;
    float cameraX;
    float playerX;
    void loadTileMap(const TileMapInfo &info);
};

#endif // TILEMAP_MANAGER_H
