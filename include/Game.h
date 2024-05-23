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
    sf::Event ev;
    Player* player;
    sf::ConvexShape triangle;

    bool showGamepadMessageFlag;
    sf::Clock messageClock;

    std::vector<Obstacle*> obstacles;
    
    void checkGamepad();

    void initWindow();
    void initObstacles(); 
    void initPlayer(); 
    void renderPlayer();
    void renderObstacles();
    void updatePlayer(float deltaTime);
    void createTriangle(bool gamepadConnected);
    
};

#endif // GAME_H