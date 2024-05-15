#ifndef ENTITY_H
#define ENTITY_H

class Entity {
    public:
        // Constructors & Destructor
        Entity();
        virtual ~Entity();

    private:
        float posX;
        float posY;

        float velX;
        float velY;

        float accX;
        float accY;
};

#endif // ENTITY_H