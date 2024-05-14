#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(){
    this->initTexture();
    this->initSprite();
    this->initAnimations();
}

Player::~Player(){

}

void Player::updateMovement(){
    this->currentState = State::Normal;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){ //Left
        this->sprite.move(-5.f, 0.f);
        this->currentState = State::Idle;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)){ //Top (Jump)
        this->sprite.move(0.f, -5.f);
        this->currentState = State::Jumping;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){ //Right
        this->sprite.move(5.f, 0.f);
        this->currentState = State::Walking;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){ //Down (Crouch)
        this->sprite.move(0.f, 5.f);
        this->currentState = State::Crouching;
    }
}


void Player::updateAnimations(float deltaTime) {
    //bool facingRight = this->sprite.getScale().x > 0; 

    int animationRow = 0; 
    switch(currentState) {
        case State::Walking: animationRow = 2; break; 
        case State::Jumping: animationRow = 5; break; 
        case State::Crouching: animationRow = 7; break;
        default: animationRow = 0; break;
    }


    animations[currentState].update(animationRow, deltaTime, true);
    this->sprite.setTextureRect(animations[currentState].uvRect);
}

void Player::update(float deltaTime) {
    this->updateMovement(); 
    if(currentState != State::Normal) {
        this->updateAnimations(deltaTime); 
    }
}


void Player::render(sf::RenderTarget & target){
    target.draw(this->sprite);
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
    animations[Idle] = Animation(&textureSheet, sf::Vector2u(4, 1), 0.3f, sf::Vector2u(32,32));       // 4 frames, 0.3 seconds per frame
    animations[Walking] = Animation(&textureSheet, sf::Vector2u(8, 2), 0.1f, sf::Vector2u(32,32));    // 8 frames, 0.1 seconds per frame
    animations[Jumping] = Animation(&textureSheet, sf::Vector2u(5, 1), 0.15f, sf::Vector2u(32,32));   // 5 frames, 0.15 seconds per frame
    animations[Crouching] = Animation(&textureSheet, sf::Vector2u(3, 1), 0.2f, sf::Vector2u(32,32));  // 3 frames, 0.2 seconds per frame
}
