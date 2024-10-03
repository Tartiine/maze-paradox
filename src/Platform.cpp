#include "Platform.h"
#include <iostream>

Platform::Platform() {
    initTexture();
    initSprite();
}

Platform::Platform(float x, float y) {
    initTexture();
    initSprite();
    setPosition(x, y);
}

Platform::~Platform() {}

sf::FloatRect Platform::getHitbox() const {
    sf::Vector2<sf::Vector2f> offset(sf::Vector2f(0, 0), sf::Vector2f(0, 7));
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    sf::FloatRect hitbox(
        bounds.left + offset.x.x,
        bounds.top + offset.y.x,
        bounds.width - (offset.x.x + offset.x.y),
        bounds.height - (offset.y.x + offset.y.y)
    );

    return hitbox;
}

void Platform::moveCollision(float x, float y) {
    setPosition(x,y);
}

void Platform::initTexture() {
    if (!textureSheet.loadFromFile("resources/sprites/platforms.png")){
        std::cerr << "Error loading platform sprite sheet" << std::endl;
    }
}

void Platform::initSprite(){
    sprite.setTexture(textureSheet);
    sprite.setTextureRect(sf::IntRect(16,16,32,16));
    sprite.setPosition(0, 0);

}