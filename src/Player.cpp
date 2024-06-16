#include "Player.h"
#include <iostream>
#include <cmath>

Player::Player(){
    initTexture();
    initSprite();
    initAnimations();
    initPhysics();
}

Player::Player(float x, float y) {
    initTexture();
    initSprite();
    initAnimations();
    initPhysics();
    setPosition(x,y);
}

Player::~Player(){
}

void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::FloatRect Player::getHitbox() const {
    sf::Vector2<sf::Vector2f> offset(sf::Vector2f(10, 10), sf::Vector2f(18, 0)); // side - top/bottom
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    sf::FloatRect hitbox(
        bounds.left + offset.x.x,
        bounds.top + offset.y.x,
        bounds.width - (offset.x.x + offset.x.y),
        bounds.height - (offset.y.x + offset.y.y)
    );

    return hitbox;
}

void Player::moveCollision(float x, float y) {
    sprite.move(x, y);
}

void Player::updateMovement(float deltaTime) {
    float DEAD_ZONE = 15.0f;

    currentState = State::Idle;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) ||
        sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -DEAD_ZONE ||
        sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -DEAD_ZONE) { // Left
        pressingLeft = true;
        currentState = State::Walking;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
               sf::Joystick::getAxisPosition(0, sf::Joystick::X) > DEAD_ZONE ||
               sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > DEAD_ZONE) { // Right
        pressingRight = true;
        currentState = State::Walking;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) ||
        sf::Joystick::isButtonPressed(0, 0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) { // Jump
        jumpReleased++;
        if (!pressingJump) {
            pressingJump = true;
            currentState = State::Jumping;
        }
    } else {
        pressingJump = false;
        jumpReleased = 0;
    }
}

void Player::updateAnimations(float deltaTime) {
    int animationRow = 0; 
    switch(currentState) {
        case State::Walking: animationRow = 1; break; 
        case State::Jumping: animationRow = 0; break; 
        case State::Crouching: animationRow = 0; break;
        default: animationRow = 0; break;
    }

    animations[currentState].update(animationRow, deltaTime); 
    sprite.setTextureRect(animations[currentState].uvRect);
}

void Player::update(float deltaTime) {
    updateMovement(deltaTime);

    updateAnimations(deltaTime); 

    updatePhysics(deltaTime);
}


void Player::render(sf::RenderTarget & target){
    target.draw(sprite);
}

void Player::updatePhysics(float deltaTime) {
    if (pressingLeft) {
        if (velocity.x <= 0) {
            velocity.x -= acceleration * deltaTime;
        } else {
            velocity.x -= deceleration * deltaTime;
        }

        if (velocity.x < -maxRunningVelocity) {
            velocity.x = -maxRunningVelocity;
        }

        sprite.setScale(-abs(sprite.getScale().x), sprite.getScale().y); 
        sprite.setOrigin(sprite.getLocalBounds().width, 0);

    } else if (pressingRight) {
        if (velocity.x >= 0) {
            velocity.x += acceleration * deltaTime;
        } else {
            velocity.x += deceleration * deltaTime;
        }

        if (velocity.x > maxRunningVelocity) {
            velocity.x = maxRunningVelocity;
        }

        sprite.setScale(abs(sprite.getScale().x), sprite.getScale().y); 
        sprite.setOrigin(0, 0);

    } else {
        if (velocity.x > 0) {
            velocity.x -= deceleration * deltaTime;
            if (velocity.x < 0) {
                velocity.x = 0;
            }
        } else if (velocity.x < 0) {
            velocity.x += deceleration * deltaTime;
            if (velocity.x > 0) {
                velocity.x = 0;
            }
        }
    }

    velocity.y += gravity * deltaTime;

    if (velocity.y > maxFallingVelocity) {
        velocity.y = maxFallingVelocity;
    }

    if (isOnGround || touchTop) {
        velocity.y = 0;
    }

    if (touchSide) {
        velocity.x = 0;
    }

    if (pressingJump && isOnGround && jumpReleased < 3) {
        velocity.y = initialJumpVelocity;
    }

    pressingLeft = false;
    pressingRight = false;

    
    sprite.move(velocity.x * deltaTime, velocity.y * deltaTime);
    
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::initTexture() {
    if (!textureSheet.loadFromFile("resources/sprites/Player/g3rar_spritesheet.png")) {
        std::cerr << "Error loading player sprite sheet" << std::endl;
    }
}

void Player::initSprite(){
    sprite.setTexture(textureSheet);
    sprite.setTextureRect(sf::IntRect(0,0,32,32));
    sprite.setPosition(0, 0);
}

void Player::initAnimations() {
    animations[Idle] = Animation(&textureSheet, sf::Vector2u(7, 1), 0.15f, sf::Vector2u(32,32));       
    animations[Walking] = Animation(&textureSheet, sf::Vector2u(6, 1), 0.09f, sf::Vector2u(32,32));    
    animations[Jumping] = Animation(&textureSheet, sf::Vector2u(7, 1), 0.15f, sf::Vector2u(32,32));   
    animations[Crouching] = Animation(&textureSheet, sf::Vector2u(7, 1), 0.2f, sf::Vector2u(32,32));  
}

void Player::initPhysics() {
    pressingLeft = false;
    pressingRight = false;
    pressingJump = false;
    jumpReleased = 0;
    

    velocity = {0, 0};
    
    maxRunningVelocity = 220.f;
    maxFallingVelocity = 1000.f;

    acceleration = 800.f;
    deceleration = 2000.f;

    gravity = 1500.f;
    airBrake = 0.8f;

    initialJumpVelocity = -400.f;
    variableJumpBoost = -5.f;
}
