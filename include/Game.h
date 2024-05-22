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

    void updatePlayer(float deltaTime);
    void update(float deltaTime);      
    
    void renderPlayer();
    void renderObstacles();
    
    void render();        
    
    const sf::RenderWindow& getWindow() const;

private:
    sf::RenderWindow window;
    sf::RenderTexture renderTexture;

    sf::Vector2i resolution {320, 180};
    unsigned int scale = 3;
    
    sf::Event ev;

    Player* player;
    std::vector<Obstacle*> obstacles;

    void initWindow();
    void initRenderTexture();
    void initObstacles(); 
    void initPlayer(); 
};

#endif // GAME_H