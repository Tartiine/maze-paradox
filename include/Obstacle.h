#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Collision.h"

/**
 * Obstacle class is the base class for every objects the Player can
 * physically interact with.
 */

class Obstacle : public Collision {
    public:
        virtual ~Obstacle() = default;
        void setPosition(float x, float y);
        void render(sf::RenderTarget & target);

    protected:
        sf::Sprite sprite;
        sf::Texture textureSheet;

        virtual void initTexture() = 0;
        virtual void initSprite() = 0;
};

#endif // OBSTACLE_H