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
    sf::Vector2f offset(0, 0);
    sf::FloatRect bounds = this->sprite.getGlobalBounds();
    
    sf::FloatRect hitbox(
        bounds.left + offset.x,
        bounds.top + offset.y,
        bounds.width - (offset.x * 2),
        bounds.height - (offset.y * 2)
    );

    return hitbox;
}


void Ground::moveCollision(float x, float y) {
    this->sprite.move(x, y); 
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