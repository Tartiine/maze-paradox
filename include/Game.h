#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

class Game {
public:
    Game(); 

    void update();          
    void render();        
    const sf::RenderWindow& getWindow() const; 

private:
    sf::RenderWindow window;    
    sf::Event ev;
    
    void initWindow(); 
};

#endif // GAME_H
