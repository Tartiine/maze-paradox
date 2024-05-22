#include "Ground.h"
#include <iostream>

using namespace std;

Ground::Ground() {
    this->initTexture();
    this->initSprite();
}

Ground::Ground(float x, float y) {
    this->initTexture();
    this->initSprite();
    this->setPosition(x, y);
}

Ground::~Ground() {

}

sf::FloatRect Ground::getHitbox() const {
    sf::Vector2<sf::Vector2f> offset(sf::Vector2f(0, 0), sf::Vector2f(0, 0));
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    
    sf::FloatRect hitbox(
        bounds.left + offset.x.x,
        bounds.top + offset.y.x,
        bounds.width - (offset.x.x + offset.x.y),
        bounds.height - (offset.y.x + offset.y.y)
    );

    return hitbox;
}

void Ground::moveCollision(float x, float y) {
    this->setPosition(x,y);
}

void Ground::initTexture() {
    if (!this->textureSheet.loadFromFile("resources/sprites/world_tileset.png")){
        cout << "Error loading ground sprite sheet" << endl;
    }
}

void Ground::initSprite(){
    this->sprite.setTexture(this->textureSheet);
    this->sprite.setTextureRect(sf::IntRect(0,0,16,16));
    this->sprite.setPosition(0, 0);

}