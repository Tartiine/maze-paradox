#ifndef GROUND_H
#define GROUND_H

#include <SFML/Graphics.hpp>
#include "Obstacle.h"

class Ground : public Obstacle {
    public:
        Ground();
        Ground(float x, float y);
        virtual ~Ground();

        sf::FloatRect getHitbox() const override;
        void moveCollision(float x, float y) override;

    private:
        void initTexture() override;
        void initSprite() override;
};

#endif // GROUND_H