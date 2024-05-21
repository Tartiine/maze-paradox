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