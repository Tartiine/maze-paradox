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
    std::vector<TileMap*> getRenderedTileMaps();
    std::unordered_map<std::string, TileMap*> getNeighbourTileMaps();
    void update(float deltaTime, Player* player,sf::RenderTarget &target);
    void render(sf::RenderTarget &target, bool debug);
    void generateTileMapOrder(const std::vector<std::string> &directories, const std::string &outputFile, int tileWidth, int tileHeight);

private:
    std::unordered_map<std::string, TileMap*> tileMaps;
    std::vector<TileMapInfo> tileMapOrder;
    TileMap* currentTileMap;
    TileMap* nextTileMap;
    TileMap* previousTileMap;
    float cameraX, cameraY; //Maybe a vector ??
    void loadTileMap(const TileMapInfo &info);
};

#endif // TILEMAP_MANAGER_H
