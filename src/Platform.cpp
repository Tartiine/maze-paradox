#include "Platform.h"
#include <iostream>

using namespace std;

Platform::Platform() {
    this->initTexture();
    this->initSprite();
}

Platform::Platform(float x, float y) {
    this->initTexture();
    this->initSprite();
    this->setPosition(x, y);
}

Platform::~Platform() {

}

void Platform::setPosition(float x, float y) {
    this->sprite.setPosition(x, y);
}

sf::FloatRect Platform::getBounds() const {
    return this->sprite.getGlobalBounds();
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
    this->sprite.setPosition(0, 0);

}

void Platform::render(sf::RenderTarget & target){
    target.draw(this->sprite);
}
