#include "Animation.h"
#include <iostream>

using namespace std;

Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, sf::Vector2u frameSize) {
    this->imageCount = imageCount;
    this->switchTime = switchTime;
    totalTime = 0.0f;
    currentImage.x = 0;

    uvRect.width = frameSize.x;
    uvRect.height = frameSize.y;
}

Animation::~Animation() {}

void Animation::update(int row, float deltaTime, bool faceRight) {
    currentImage.y = row;
    totalTime += deltaTime;
              
    if (totalTime >= switchTime) {
        totalTime -= switchTime;
        currentImage.x++;

        if (currentImage.x >= imageCount.x) {
            currentImage.x = 0;
        }
    }

    uvRect.left = currentImage.x * uvRect.width;
    uvRect.top = currentImage.y * uvRect.height;

    if (faceRight) {
        uvRect.width = abs(uvRect.width);
    } else {
        uvRect.width = -abs(uvRect.width);
    }
}
