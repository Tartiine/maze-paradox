#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
    public:
        // Constructors & Destructor
        Entity();
        virtual ~Entity();

    private:
        float posX;
        float posY;
        sf::Vector2f velocity;
        float maxVelocity;
        float minVelocity;
        float acceleration;
        float deceleration;

};

#endif // ENTITY_H