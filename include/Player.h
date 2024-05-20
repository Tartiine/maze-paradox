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
        Crouching,
        Falling
    };

    Player();
    virtual ~Player();

    void moveCollision(float x, float y);

    void move(const float dir_x, const float dir_y, float deltaTime);
    void updateMovement(float deltaTime);
    void updateAnimations(float deltaTime);
    void update(float deltaTime);
    void render(sf::RenderTarget& target);
    void updatePhysics();

    bool isColliding(const sf::FloatRect &other) const;
    void resolveCollision(const sf::FloatRect &other);
    //sf::FloatRect getHitbox() const; //TODO: QQUE CHOSE COMME CA

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
    float gravity;
    float maxVelocityY;
    float jumpHeight;
    bool canJump = true;
    bool keyPressed = false;
    float jumpVelocity;  
    float airControl;    
    float airBrake; 
    float adjustmentFactor = 10.f;

    //sf::FloatRect hitboxOffset; //TODO: Definir un offest ou sinon directement dans le code (a voir)
    /*ou sinon si t'a besoin de mettre des offsets que sur certains 
    cotés tu peux ptet juste faire des float offsetX ou qque chose comme ca*/
    //TODO: Faire la meme chose dans platform !!!
};

#endif // PLAYER_H