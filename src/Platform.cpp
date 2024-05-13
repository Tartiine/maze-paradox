#include "Platform.h"
#include <iostream>

using namespace std;

Platform::Platform() {
    this->initTexture();
    this->initSprite();
}

Platform::~Platform() {

}

void Platform::initTexture() {
    if (!this->textureSheet.loadFromFile("resources/sprites/platforms.png")){
        cout << "Error loading platform sprite sheet" << endl;
    }
}

void Platform::initSprite(){
    this->sprite.setTexture(this->textureSheet);
    this->sprite.setTextureRect(sf::IntRect(0,0,16,16));

    this->sprite.setScale(3.0f,3.0f);
    this->sprite.setPosition(400, 300);

}

void Platform::render(sf::RenderTarget & target){
    target.draw(this->sprite);
}
