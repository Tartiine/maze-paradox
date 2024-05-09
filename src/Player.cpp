#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(){
    this->initTexture();
    this->initSprite();
}

Player::~Player(){

}

void Player::updateMovement(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){ //Left
        this->sprite.move(-5.f, 0.f);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)){ //Top (Jump)
        this->sprite.move(0.f, -5.f);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){ //Right
        this->sprite.move(5.f, 0.f);
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){ //Down (Crouch)
        this->sprite.move(0.f, 5.f);
    }
}

void Player::update(){
    this->updateMovement();
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

}
