#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Obstacle.h"
#include "TileMap.h"
#include "TileMapManager.h"
#include "TileMapGenerator.h"
#include <vector>
#include <memory>
#include "TileMapModel.h"

class Game {
public:
    Game(); 
    virtual ~Game();

    void collisionPlayer();
    void update(float deltaTime);      
    void render();        
    
    const sf::RenderWindow& getWindow() const;

private:
    sf::RenderWindow window;
    sf::RenderTexture renderTexture;

    sf::Vector2i resolution {640, 360};
    unsigned int scale = 2;
    
    Player* player;
    TileMap* tileMap;
    TileMapManager* tileMapManager;
    TileMapGenerator* generator;
    std::vector<Obstacle*> obstacles;
    
    bool isFullscreenOn;
    int fullscreenHorizontalOffset;
    int fullscreenVerticalOffset;

    std::vector<TileMap*> tileMaps;
    int currentTileMapIndex;
    float playerTilePosition;

    TileMapModel* tileMapModel;

    sf::ConvexShape triangle;
    bool showGamepadFlag;
    sf::Clock infoClock;

    void initWindow();
    void initWindowFullscreen();
    void initRenderTexture();
    void initObstacles(); 
    void initPlayer();  
    void initMap();
    void renderPlayer();
    void renderObstacles(bool debug);
    void updatePlayer(float deltaTime);
    void createTriangle(bool gamepadConnected);
    void checkGamepad();
};

#endif // GAME_H