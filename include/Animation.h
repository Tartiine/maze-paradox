#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>

class Animation {
public:
    Animation() : switchTime(0.0f), totalTime(0.0f), currentImage(0, 0) {}
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
