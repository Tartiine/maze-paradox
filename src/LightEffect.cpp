#include "LightEffect.h"

LightEffect::LightEffect(const sf::Vector2f& position, float radius, float duration)
    : remainingTime(duration), initialDuration(duration) {
    light.setRadius(radius * 1.5f); 
    light.setOrigin(light.getRadius(), light.getRadius()); 
    light.setPosition(position);
    light.setFillColor(sf::Color(255, 255, 0, 100));
}

void LightEffect::update(float deltaTime) {
    remainingTime -= deltaTime;

    if (remainingTime > 0.0f) {
        float alpha = static_cast<int>(255 * (remainingTime / initialDuration));
        light.setFillColor(sf::Color(255, 255, 0, alpha));
    }
}

void LightEffect::render(sf::RenderTarget& target) const {
    if (remainingTime > 0.0f) {
        target.draw(light);
    }
}

bool LightEffect::isExpired() const {
    return remainingTime <= 0.0f;
}
