#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "Animation.h"

class HealthComponent {
private:
    int maxHP;
    int currentHP;
    sf::Texture heartTexture;
    sf::Sprite heartSprite;
    
    Animation hurtAnimation;
    Animation healAnimation;

    bool playingHurtAnimation = false;
    bool playingHealAnimation = false;
    float animationTimer = 0.0f;

public:
    HealthComponent(int maxHealth);

    void initHeart();
    void renderHearts(sf::RenderTarget & target);
    void update(float deltaTime);

    void takeDamage(int damage);
    void heal(int amount);
    int getHealth() const;
    int getMaxHealth() const;
    bool isDead() const;

    void reset(); 
};

#endif
