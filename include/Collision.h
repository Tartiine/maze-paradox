#ifndef COLLISION_H
#define COLLISION_H

#include <SFML/Graphics.hpp>

/**
 * Collision class is an abstract class that allow every inherited children
 * to detect and resolve collisions between them (such as the Player with an Obstacle).
 */

class Collision {
    public:
        virtual ~Collision() = default;

        virtual sf::FloatRect getHitbox() const = 0;
        virtual void moveCollision(float x, float y) = 0;

        bool isColliding(const sf::FloatRect &other) const;
        void resolveCollision(const sf::FloatRect &other);
        void checkWindowBorders(const sf::RenderTexture& window);
        
    protected :
        bool isOnGround = false;
        bool touchSide = false;
        bool touchTop = false;

};

#endif // COLLISION_H