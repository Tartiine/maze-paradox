#include "Obstacle.h"

void Obstacle::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Obstacle::render(sf::RenderTarget &target) {
    target.draw(sprite);
}
