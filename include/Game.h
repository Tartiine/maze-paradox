#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Obstacle.h"
#include "TileMap.h"
#include "TileMapManager.h"

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
    Player* player;
    TileMap* tileMap;
    TileMapManager* tileMapManager;
    std::vector<Obstacle*> obstacles;
    std::vector<TileMap*> tileMaps;
    int currentTileMapIndex;
    float playerTilePosition;

    sf::ConvexShape triangle;
    bool showGamepadFlag;
    sf::Clock infoClock;

    void initWindow();
    void initObstacles(); 
    void initPlayer(); 
    void renderPlayer();
    void renderObstacles(bool debug);
    void updatePlayer(float deltaTime);
    void createTriangle(bool gamepadConnected);
    void checkGamepad();
};

#endif // GAME_H