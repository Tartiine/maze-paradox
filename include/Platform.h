#ifndef PLATFORM_H
#define PLATFORM_H

#include "Obstacle.h"

class Platform : public Obstacle {
    public:
        Platform();
        Platform(float x, float y);
        virtual ~Platform();

        sf::FloatRect getHitbox() const override;
        void moveCollision(float x, float y) override;

    private:
        void initTexture() override;
        void initSprite() override;
};

#endif // PLATFORM_H