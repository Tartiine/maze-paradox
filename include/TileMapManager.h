#ifndef TILEMAP_MANAGER_H
#define TILEMAP_MANAGER_H

#include "TileMap.h"
#include "Player.h"
#include "Animation.h"
#include "LightEffect.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <cstdint>
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
    void loadTileMaps(const std::vector<std::vector<std::vector<uint8_t>>> &tileMapBatches);
    std::vector<TileMap *> getRenderedTileMaps();
    std::unordered_map<std::string, TileMap*> getNeighbourTileMaps();
    void update(float deltaTime, Player* player, sf::RenderTarget &window);
    void render(sf::RenderTarget &target, bool debug);
    void generateTileMapOrder(const std::vector<std::vector<std::vector<uint8_t>>> &tileMapBatches, int tileWidth, int tileHeight);
    void createFinalMap();
    void updateAnimation(float deltaTime);
    void render(sf::RenderTarget& target);
    bool checkPortal(Player* player);
    void deletePlatform(sf::Vector2f playerPosition);
    sf::Vector2i getTileToDelete() const;

private:
    std::unordered_map<std::string, std::unique_ptr<TileMap>> tileMaps;
    std::vector<TileMapInfo> tileMapOrder;
    std::vector<LightEffect> lightEffects;
    TileMap* currentTileMap;
    TileMap* nextTileMap;
    TileMap* previousTileMap;
    float cameraX, cameraY;
    void loadTileMap(const TileMapInfo &info, const std::vector<uint8_t> &binaryData);
    sf::Sprite portalSprite;
    std::unique_ptr<Animation> portalAnimation;
    sf::Texture portalTexture;
    float deletionTimer = 0.0f; 
    bool pendingDeletion = false; 
    int tileXToDelete = -1, tileYToDelete = -1; 
};

#endif // TILEMAP_MANAGER_H
