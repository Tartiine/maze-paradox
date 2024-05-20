#include "Obstacle.h"

void Obstacle::setPosition(float x, float y) {
    this->sprite.setPosition(x, y);
}

void Obstacle::render(sf::RenderTarget &target) {
    target.draw(this->sprite);
}
