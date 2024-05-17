#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>

class Platform{
    public:
        Platform();
        Platform(float x, float y);
        virtual ~Platform();

        void setPosition(float x, float y);

        void render(sf::RenderTarget & target);

    private:
        sf::Sprite sprite;
        sf::Texture textureSheet;

        void initTexture();
        void initSprite();
};

#endif // PLATFORM_H