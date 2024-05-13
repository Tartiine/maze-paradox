#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Player.h"

class Game {
public:
    Game(); 
    virtual ~Game();

    void updatePlayer(float deltaTime);
    void update(float deltaTime);      
    void renderPlayer();    
    void render();        
    const sf::RenderWindow& getWindow() const; 

private:
    sf::RenderWindow window;    
    sf::Event ev;

    Player* player;
    
    void initWindow(); 
    void initPlayer(); 
};

#endif // GAME_H
