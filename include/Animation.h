#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

/**
 * Animation class that stores the different animation sprites of an object
 * as well as updating the sprite to display based on a set timestamp.
 */

class Animation {
public:
    Animation();
    Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, sf::Vector2u frameSize);
    ~Animation();

    void update(int row, float deltaTime);

    sf::IntRect uvRect;

private:
    sf::Vector2u imageCount;
    sf::Vector2u currentImage;

    float totalTime;
    float switchTime;
};

#endif // ANIMATION_H
