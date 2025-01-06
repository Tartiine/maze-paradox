#ifndef LIGHT_EFFECT_H
#define LIGHT_EFFECT_H

#include <SFML/Graphics.hpp>

class LightEffect {
public:
    LightEffect(const sf::Vector2f& position, float radius, float duration);

    sf::Vector2f getPosition() const {
        return light.getPosition();
    }

    void update(float deltaTime);
    void render(sf::RenderTarget& target) const;

    bool isExpired() const;

private:
    sf::CircleShape light;
    float remainingTime;
    float initialDuration;
};

#endif // LIGHT_EFFECT_H
