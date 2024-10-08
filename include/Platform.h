#ifndef PLATFORM_H
#define PLATFORM_H

#include "Obstacle.h"

/**
 * Platform class represents blocs of ground that are in the air.
 * The Player can jump on it and move around a level vertically thanks to this.
 */

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