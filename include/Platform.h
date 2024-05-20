#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include "Collision.h"

class Platform : public Collision {
    public:
        Platform();
        Platform(float x, float y);
        virtual ~Platform();

        sf::FloatRect getHitbox() const override;
        void moveCollision(float x, float y) override;

        void setPosition(float x, float y);

        void render(sf::RenderTarget & target);

    private:
        sf::Sprite sprite;
        sf::Texture textureSheet;

        void initTexture();
        void initSprite();
};

#endif // PLATFORM_H