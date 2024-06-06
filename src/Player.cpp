#include "Player.h"
#include <iostream>
#include <cmath>

using namespace std;

Player::Player(){
    this->initTexture();
    this->initSprite();
    this->initAnimations();
    this->initPhysics();
}

Player::~Player(){
}

sf::FloatRect Player::getHitbox() const {
    sf::Vector2<sf::Vector2f> offset(sf::Vector2f(30, 30), sf::Vector2f(30, 12));
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    
    sf::FloatRect hitbox(
        bounds.left + offset.x.x,
        bounds.top + offset.y.x,
        bounds.width - (offset.x.x + offset.x.y),
        bounds.height - (offset.y.x + offset.y.y)
    );

    return hitbox;
}

void Player::moveCollision(float x, float y) {
    this->sprite.move(x, y);
}

void Player::move(const float dir_x, const float dir_y, float deltaTime){
        this->velocity.x += dir_x * this->acceleration * deltaTime;
        this->velocity.y += dir_y * this->acceleration * deltaTime;
        if(abs(this->velocity.x) > this->maxVelocity) {
            this->velocity.x = this->maxVelocity * ((this->velocity.x < 0.f) ? -1.f : 1.f);
        }

        if (dir_x < 0.f) { 
            this->sprite.setScale(-abs(this->sprite.getScale().x), this->sprite.getScale().y); 
            this->sprite.setOrigin(this->sprite.getLocalBounds().width, 0); 
        } else if (dir_x > 0.f) { 
            this->sprite.setScale(abs(this->sprite.getScale().x), this->sprite.getScale().y); 
            this->sprite.setOrigin(0, 0);
        }

}

void Player::updateMovement(float deltaTime) {
    float DEAD_ZONE = 15.0f;

    this->currentState = State::Idle;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) ||
        sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -DEAD_ZONE ||
        sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) < -DEAD_ZONE) { // Left
        this->move(-5.f, 0.f, deltaTime);
        this->currentState = State::Walking;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
               sf::Joystick::getAxisPosition(0, sf::Joystick::X) > DEAD_ZONE ||
               sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) > DEAD_ZONE) { // Right
        this->move(5.f, 0.f, deltaTime);
        this->currentState = State::Walking;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) ||
        sf::Joystick::isButtonPressed(0, 0) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) { // Jump
        if (!this->keyPressed && this->isOnGround) {
            this->velocity.y = -sqrtf(2.0f * this->gravity * this->jumpHeight);
            this->currentState = State::Jumping;
            this->keyPressed = true;
        }
    } else {
        this->keyPressed = false; 
    }
}

void Player::updateAnimations(float deltaTime) {
    int animationRow = 0; 
    switch(currentState) {
        case State::Walking: animationRow = 2; break; 
        case State::Jumping: animationRow = 5; break; 
        case State::Crouching: animationRow = 7; break;
        default: animationRow = 0; break;
    }

    animations[currentState].update(animationRow, deltaTime); 
    this->sprite.setTextureRect(animations[currentState].uvRect);
}

void Player::update(float deltaTime) {
    this->updateMovement(deltaTime);

    this->updateAnimations(deltaTime); 

    this->updatePhysics();

    this->isOnGround = false;

}


void Player::render(sf::RenderTarget & target){
    target.draw(this->sprite);
}

void Player::updatePhysics() {
    //Gravity
    if(!isOnGround) {
        this->velocity.y += 1.0 * this->gravity;
    }else{
        this->velocity *= this->deceleration;
    }

    if(abs(this->velocity.x) > this->maxVelocityY) {
        this->velocity.y = this->maxVelocityY * ((this->velocity.y < 0.f) ? -1.f : 1.f);
    }
 
    if(abs(this->velocity.x) < this->minVelocity)
        this->velocity.x = 0.f;
    if(abs(this->velocity.y) < this->minVelocity)
        this->velocity.y = 0.f;
    this->sprite.move(this->velocity);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::initTexture(){
    if(!this->textureSheet.loadFromFile("resources/sprites/knight.png")){
        cout << "Error loading player sprite sheet" << endl;
    }

}

void Player::initSprite(){
    this->sprite.setTexture(this->textureSheet);
    this->sprite.setTextureRect(sf::IntRect(0,0,32,32));

    this->sprite.setScale(3.0f,3.0f);
    this->sprite.setPosition(10.f, 210.f);
}

void Player::initAnimations() {
    animations[Idle] = Animation(&textureSheet, sf::Vector2u(4, 1), 0.15f, sf::Vector2u(32,32));       
    animations[Walking] = Animation(&textureSheet, sf::Vector2u(8, 2), 0.09f, sf::Vector2u(32,32));    
    animations[Jumping] = Animation(&textureSheet, sf::Vector2u(5, 1), 0.15f, sf::Vector2u(32,32));   
    animations[Crouching] = Animation(&textureSheet, sf::Vector2u(3, 1), 0.2f, sf::Vector2u(32,32));  
}

void Player::initPhysics() {
    this->maxVelocity = 7.f;
    this->minVelocity = 1.f;
    this->acceleration = 120.f;
    this->deceleration = 0.7f;
    this->gravity = 2.f;
    this->maxVelocityY = 10.f;
    this->jumpHeight = 120.f;
    this->jumpVelocity = 0.0f;
    /*
    this->airControl = 200.0f; 
    this->airBrake = 0.5f;
    */ 
}

//TODO: variable height jump
