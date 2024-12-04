#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Obstacle.h"
#include "TileMap.h"
#include "TileMapModel.h"
#include "TileMapManager.h"
#include "TileMapGenerator.h"
#include <vector>
#include <memory>

/**
 * Game class is the main class. It handles every other class and allows them to communicate.
 * It manages every objects initialization, as well as updating the rendering.
 */

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

    sf::Vector2i resolution {640, 352};
    unsigned int scale = 2;
    
    std::unique_ptr<Player> player;
    std::unique_ptr<TileMap> tileMap;
    std::unique_ptr<TileMapManager> tileMapManager;
    std::unique_ptr<TileMapGenerator> generator;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    
    bool isFullscreenOn;
    int fullscreenHorizontalOffset;
    int fullscreenVerticalOffset;

    std::vector<std::unique_ptr<TileMap>> tileMaps;

    std::unique_ptr<TileMapModel> tileMapModel;

    sf::ConvexShape triangle;
    bool showGamepadFlag;
    sf::Clock infoClock;

    bool gameStarted;
    sf::Font startFont;
    sf::Text startText;
    sf::Font secondaryFont;
    sf::Text secondaryText;
    sf::Text gamepadText;
    sf::RectangleShape startBackground;

    void loadFonts();
    void initStartScreen();
    void initWindow();
    void initWindowFullscreen();
    void initRenderTexture();
    void initPlayer();  
    void initMap();
    void renderPlayer();
    void renderObstacles(bool debug);
    void updatePlayer(float deltaTime);
    void createGamepadText(bool gamepadConnected);
    void checkGamepad();
    void trainModel();
    void showEndMenu();
    void resetGame();
};

#endif // GAME_H
