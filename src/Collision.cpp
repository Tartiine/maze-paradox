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

    this->isOnGround = false;

    this->touchSide = false;
    this->touchTop = false;

    if (fabs(minOverlapX) < fabs(minOverlapY)) {
        touchSide = true;
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
            touchTop = true; 
        }
    }
}

void Collision::checkWindowBorders(const sf::RenderTexture &renderTexture) {
    sf::FloatRect hitbox = getHitbox();
    sf::Vector2f position = sf::Vector2f(hitbox.left, hitbox.top);
    sf::Vector2f size = sf::Vector2f(hitbox.width, hitbox.height);

    this->isOnGround = false;

    this->touchSide = false;
    this->touchTop = false;

    sf::View view = renderTexture.getView();
    sf::FloatRect viewBounds(view.getCenter() - view.getSize() / 2.f, view.getSize());

    if (position.x < viewBounds.left) {
        touchSide = true;
        float adjustment = viewBounds.left - position.x;
        moveCollision(adjustment, 0);
    }
    if (position.x + size.x > viewBounds.left + viewBounds.width) {
        touchSide = true;
        float adjustment = (viewBounds.left + viewBounds.width) - (position.x + size.x);
        moveCollision(adjustment, 0);
    }
    if (position.y < viewBounds.top) {
        touchTop = true; 
        float adjustment = viewBounds.top - position.y;
        moveCollision(0, adjustment);
    }
    if (position.y + size.y > viewBounds.top + viewBounds.height) {
        isOnGround = true;
        float adjustment = (viewBounds.top + viewBounds.height) - (position.y + size.y);
        moveCollision(0, adjustment);
    }

    hitbox = getHitbox();
    position = sf::Vector2f(hitbox.left, hitbox.top);

}