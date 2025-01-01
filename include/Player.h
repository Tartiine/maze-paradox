#ifndef PLAYER_H
#define PLAYER_H

#include <unordered_map>
#include "Animation.h"
#include "Collision.h"

/**
 * Player class that manages all aspects of the player behaviour,
 * such as movement, physics, rendering and handling inputs.
 */

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

    bool hasJumped(); 
    void changePhysics(float duration);
    void updatePhysicsEffect(float deltaTime);

private:
    sf::Sprite sprite;
    sf::Texture textureSheet;
    State currentState = State::Idle;
    sf::Vector2f originalScale;
    sf::Vector2f originalPosition;
    
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
    bool jumpOccurred;

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

    float physicsEffectRemainingTime = 0.0f;
    bool physicsChanged = false;
};

#endif // PLAYER_H