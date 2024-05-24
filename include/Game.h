#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Obstacle.h"

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
    
    sf::Event ev;
    Player* player;
    sf::ConvexShape triangle;

    bool showGamepadMessageFlag;
    sf::Clock messageClock;

    std::vector<Obstacle*> obstacles;
    
    bool isFullscreenOn;
    int fullscreenVerticalOffset; 

    void checkGamepad();

    void initWindow();
    void initWindowFullscreen();
    void initRenderTexture();
    void initObstacles(); 
    void initPlayer(); 
    void renderPlayer();
    void renderObstacles();
    void updatePlayer(float deltaTime);
    void createTriangle(bool gamepadConnected);
    
};

#endif // GAME_H