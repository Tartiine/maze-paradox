#ifndef LIGHTEFFECT_H
#define LIGHTEFFECT_H

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

#endif // LIGHTEFFECT_H
