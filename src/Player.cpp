#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(){
    this->initTexture();
    this->initSprite();
    this->initAnimations();
    this->initPhysics();
}

Player::~Player(){
}

void Player::move(const float dir_x, const float dir_y){
        this->velocity.x += dir_x * this->acceleration;
        //this->velocity.y += dir_y * this->acceleration;
        if(abs(this->velocity.x) > this->maxVelocity) {
            this->velocity.x = this->maxVelocity * ((this->velocity.x < 0.f) ? -1.f : 1.f);
        }
}

void Player::updateMovement()
{
    this->currentState = State::Normal;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){ //Left
        this->move(-5.f, 0.f);
        this->currentState = State::Idle;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)){ //Top (Jump)
        this->move(0.f, -5.f);
        this->currentState = State::Jumping;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){ //Right
        this->move(5.f, 0.f);
        this->currentState = State::Walking;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){ //Down (Crouch)
        this->move(0.f, 5.f);
        this->currentState = State::Crouching;
    }
}

void Player::updateAnimations(float deltaTime) {
    bool facingRight = this->sprite.getScale().x > 0; 

    int animationRow = 0; 
    switch(currentState) {
        case State::Walking: animationRow = 2; break; 
        case State::Jumping: animationRow = 5; break; 
        case State::Crouching: animationRow = 7; break;
        default: animationRow = 0; break;
    }


    animations[currentState].update(animationRow, deltaTime, facingRight); //TODO: Fix facingRight
    this->sprite.setTextureRect(animations[currentState].uvRect);
}

void Player::update(float deltaTime) {
    this->updateMovement(); 
    if(currentState != State::Normal) {
        this->updateAnimations(deltaTime); 
    }
    this->updatePhysics();
}

void Player::render(sf::RenderTarget & target){
    target.draw(this->sprite);
}

void Player::updatePhysics() {
    this->velocity *= this->deceleration;
    if(abs(this->velocity.x) < this->minVelocity)
        this->velocity.x = 0.f;
    if(abs(this->velocity.y) < this->minVelocity)
        this->velocity.y = 0.f;
    
    this->sprite.move(this->velocity);
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
}

void Player::initAnimations() {
    animations[Idle] = Animation(&textureSheet, sf::Vector2u(4, 1), 0.3f, sf::Vector2u(32,32));       
    animations[Walking] = Animation(&textureSheet, sf::Vector2u(8, 2), 0.09f, sf::Vector2u(32,32));    
    animations[Jumping] = Animation(&textureSheet, sf::Vector2u(5, 1), 0.15f, sf::Vector2u(32,32));   
    animations[Crouching] = Animation(&textureSheet, sf::Vector2u(3, 1), 0.2f, sf::Vector2u(32,32));  
}

void Player::initPhysics() {
    this->maxVelocity = 7.f;
    this->minVelocity = 1.f;
    this->acceleration = 2.f;
    this->deceleration = 0.9f;
}
