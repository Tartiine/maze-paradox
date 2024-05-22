#include "Collision.h"
#include <cmath>

using namespace std;

bool Collision::isColliding(const sf::FloatRect &other) const {
    return this->getHitbox().intersects(other);
}

void Collision::resolveCollision(const sf::FloatRect &other) {
    sf::FloatRect hitbox = this->getHitbox();

    float overlapLeft = (hitbox.left + hitbox.width) - other.left;
    float overlapRight = (other.left + other.width) - hitbox.left;
    float overlapTop = (hitbox.top + hitbox.height) - other.top;
    float overlapBottom = (other.top + other.height) - hitbox.top;

    bool fromLeft = fabs(overlapLeft) < fabs(overlapRight);
    bool fromTop = fabs(overlapTop) < fabs(overlapBottom);

    float minOverlapX = fromLeft ? overlapLeft : overlapRight;
    float minOverlapY = fromTop ? overlapTop : overlapBottom;

    if (fabs(minOverlapX) < fabs(minOverlapY)) {
        if (fromLeft) {
            this->moveCollision(-minOverlapX, 0);
        } else {
            this->moveCollision(minOverlapX, 0);
        }
    } else {
        if (fromTop) {
            this->moveCollision(0, -minOverlapY);
            isOnGround = true;
        } else {
            this->moveCollision(0, minOverlapY); 
        }
    }
}

void Collision::checkWindowBorders(const sf::RenderWindow& window) {
    sf::FloatRect hitbox = getHitbox();
    sf::Vector2f position = sf::Vector2f(hitbox.left, hitbox.top);
    sf::Vector2f size = sf::Vector2f(hitbox.width, hitbox.height);

    if (position.x < 0) {
        position.x = 0;
        moveCollision(-hitbox.left, 0); 
    }
    if (position.x + size.x > window.getSize().x) {
        position.x = window.getSize().x - size.x;
        moveCollision(window.getSize().x - (hitbox.left + hitbox.width), 0); 
    }
    if (position.y < 0) {
        position.y = 0;
        moveCollision(0, -hitbox.top); 
    }
    if (position.y + size.y > window.getSize().y) {
        position.y = window.getSize().y - size.y;
        moveCollision(0, window.getSize().y - (hitbox.top + hitbox.height)); 
        isOnGround = true; 
    }
}
