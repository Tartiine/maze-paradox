#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Animation.h"
#include "Collision.h"

class Player : public Collision {
public:
    enum State {
        Idle,
        Walking,
        Jumping,
        Crouching,
        Falling
    };

    Player();
    virtual ~Player();

    sf::FloatRect getHitbox() const override;
    void moveCollision(float x, float y) override;

    void move(const float dir_x, const float dir_y, float deltaTime);
    void updateMovement(float deltaTime);
    void updateAnimations(float deltaTime);
    void update(float deltaTime);
    void render(sf::RenderTarget& target);
    void updatePhysics();
    sf::Vector2f getPosition() const;

private:
    sf::Sprite sprite;
    sf::Texture textureSheet;
    State currentState = State::Idle;

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
    float gravity;
    float maxVelocityY;
    float jumpHeight;
    bool keyPressed = false;
    float jumpVelocity;  
    float airControl;    
    float airBrake; 
    float adjustmentFactor = 10.f;
    bool facingRight = false;

};

#endif // PLAYER_H