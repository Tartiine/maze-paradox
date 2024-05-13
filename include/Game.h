#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Platform.h"

class Game {
public:
    Game(); 
    virtual ~Game();

    void updatePlayer(float deltaTime);
    void update(float deltaTime);      
    
    void renderPlayer();
    void renderPlatform();    
    
    void render();        
    
    const sf::RenderWindow& getWindow() const; 

private:
    sf::RenderWindow window;    
    sf::Event ev;

    Player* player;
    Platform* platform;
    
    void initWindow();
    void initPlatform(); 
    void initPlayer(); 
};

#endif // GAME_H