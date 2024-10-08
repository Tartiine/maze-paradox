#ifndef TILEMAP_MANAGER_H
#define TILEMAP_MANAGER_H

#include "TileMap.h"
#include "Player.h"
#include "Animation.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

struct TileMapInfo {
    std::string filename;
    sf::Vector2f position;

    TileMapInfo(const std::string& file, const sf::Vector2f& pos)
        : filename(file), position(pos) {}
};

/**
 * TileManager class manages the loading and the rendering of the maps.
 * It also handles the camera movement in order to follow the Player.
 */

class TileMapManager {
public:
    TileMapManager();
    ~TileMapManager();
    void loadTileMaps(const std::string &fileName);
    std::vector<TileMap*> getRenderedTileMaps();
    std::unordered_map<std::string, TileMap*> getNeighbourTileMaps();
    void update(float deltaTime, Player* player, sf::RenderTarget &window);
    void render(sf::RenderTarget &target, bool debug);
    void generateTileMapOrder(const std::vector<std::string> &directories, const std::string &outputFile, int tileWidth, int tileHeight);
    void createFinalMap();
    void updateAnimation(float deltaTime);
    void render(sf::RenderTarget& target);
    bool checkPortal(Player* player);

private:
    std::unordered_map<std::string, std::unique_ptr<TileMap>> tileMaps;
    std::vector<TileMapInfo> tileMapOrder;
    TileMap* currentTileMap;
    TileMap* nextTileMap;
    TileMap* previousTileMap;
    float cameraX, cameraY;
    void loadTileMap(const TileMapInfo &info);
    sf::Sprite portalSprite;
    std::unique_ptr<Animation> portalAnimation;
    sf::Texture portalTexture;
};

#endif // TILEMAP_MANAGER_H
