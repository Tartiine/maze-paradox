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
    Player(float x, float y);
    virtual ~Player();

    void setPosition(float x, float y);

    sf::FloatRect getHitbox() const override;
    void moveCollision(float x, float y) override;

    void move(const float dir_x, const float dir_y, float deltaTime);
    void updateMovement(float deltaTime);
    void updateAnimations(float deltaTime);
    void update(float deltaTime);
    void render(sf::RenderTarget& target);
    void updatePhysics(float deltaTime);

private:
    sf::Sprite sprite;
    sf::Texture textureSheet;
    State currentState = State::Idle;

    std::unordered_map<State, Animation> animations;

    void initTexture();
    void initSprite();
    void initAnimations();
    void initPhysics();

    // Inputs
    bool pressingLeft;
    bool pressingRight;
    bool pressingJump;


    // Physics
    sf::Vector2f velocity;

    float maxRunningVelocity;
    float maxFallingVelocity;
    
    float acceleration; // on the ground
    float deceleration;

    float gravity;  // in the air
    float airBrake;

    float initialJumpVelocity;
    float variableJumpBoost;
    
    float jumpHoldTime;
    bool jumpKeyHeld;
    float jumpTime;

    /*
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
    */
    
    
    // TODO: Ajuster les valeurs à la nouvelle résolution
};

//TODO: Add getters and setters

#endif // PLAYER_H