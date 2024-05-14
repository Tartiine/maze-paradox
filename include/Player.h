#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Animation.h"

class Player{
public:
    enum State {
        Normal,
        Idle,
        Walking,
        Jumping,
        Crouching
    };

    Player();
    virtual ~Player();

    void updateMovement();
    void updateAnimations(float deltaTime);
    void update(float deltaTime);
    void render(sf::RenderTarget& target);

private:
    sf::Sprite sprite;
    sf::Texture textureSheet;
    State currentState;

    std::unordered_map<State, Animation> animations;

    void initTexture();
    void initSprite();
    void initAnimations();
};

#endif // PLAYER_H