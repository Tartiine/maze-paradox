#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Platform.h"

class Game {
public:
    Game(); 
    virtual ~Game();

    void collisionPlayer();

    void updatePlayer(float deltaTime);
    void update(float deltaTime);      
    
    void renderPlayer();
    void renderPlatforms();    
    
    void render();        
    
    const sf::RenderWindow& getWindow() const; 

private:
    sf::RenderWindow window;    
    sf::Event ev;

    Player* player;
    std::vector<Platform*> platforms;
    
    void initWindow();
    void initPlatforms(); 
    void initPlayer(); 
};

#endif // GAME_H