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

sf::FloatRect Platform::getHitbox() const {
    sf::Vector2<sf::Vector2f> offset(sf::Vector2f(0, 0), sf::Vector2f(0, 21));
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    
    sf::FloatRect hitbox(
        bounds.left + offset.x.x,
        bounds.top + offset.y.x,
        bounds.width - (offset.x.x + offset.x.y),
        bounds.height - (offset.y.x + offset.y.y)
    );

    return hitbox;
}

void Platform::moveCollision(float x, float y) {
    this->setPosition(x,y);
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