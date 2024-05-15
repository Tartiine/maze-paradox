#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Animation.h"

class Player {
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

    void move(const float dir_x, const float dir_y);
    void updateMovement();
    void updateAnimations(float deltaTime);
    void update(float deltaTime);
    void render(sf::RenderTarget& target);
    void updatePhysics();

private:
    sf::Sprite sprite;
    sf::Texture textureSheet;
    State currentState;

    std::unordered_map<State, Animation> animations;

    void initTexture();
    void initSprite();
    void initAnimations();
    void initPhysics();

    sf::Vector2f velocity;
    float maxVelocity;
    float minVelocity;
    float acceleration;
    float deceleration;

};

#endif // PLAYER_H