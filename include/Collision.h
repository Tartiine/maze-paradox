#ifndef COLLISION_H
#define COLLISION_H

#include <SFML/Graphics.hpp>

class Collision {
    public:

        virtual sf::FloatRect getHitbox() const = 0;
        virtual void moveCollision(float x, float y) = 0;

        bool isColliding(const sf::FloatRect &other) const;
        void resolveCollision(const sf::FloatRect &other);

    protected :
        bool isOnGround = false;

};

#endif // COLLISION_H