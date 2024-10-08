#ifndef GROUND_H
#define GROUND_H

#include <SFML/Graphics.hpp>
#include "Obstacle.h"

/**
 * Ground class represents blocs of ground the Player can run on.
 * They are also used at the bottom of the map to prevent free fall.
 */

class Ground : public Obstacle {
    public:
        Ground();
        Ground(float x, float y);
        virtual ~Ground() override;

        sf::FloatRect getHitbox() const override;
        void moveCollision(float x, float y) override;

    private:
        void initTexture() override;
        void initSprite() override;
};

#endif // GROUND_H