#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Collision.h"

class Obstacle : public Collision {
    public:
        void setPosition(float x, float y);
        void render(sf::RenderTarget & target);

    protected:
        sf::Sprite sprite;
        sf::Texture textureSheet;

        virtual void initTexture() = 0;
        virtual void initSprite() = 0;
};

#endif // OBSTACLE_H