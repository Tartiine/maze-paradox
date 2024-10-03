#ifndef PLAYER_H
#define PLAYER_H

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

    void updateMovement(float deltaTime);
    void updateAnimations(float deltaTime);
    void update(float deltaTime);
    void render(sf::RenderTarget& target);
    void updatePhysics(float deltaTime);
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

    // Inputs
    bool pressingLeft;
    bool pressingRight;
    bool pressingJump;
    unsigned int jumpReleased;


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

};

#endif // PLAYER_H