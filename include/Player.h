#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player{
public:
    Player();
    virtual ~Player();

    void updateMovement();
    void update();
    void render(sf::RenderTarget& target);

private:
    sf::Sprite sprite;
    sf::Texture textureSheet;

    void initTexture();
    void initSprite();
};

#endif // PLAYER_H