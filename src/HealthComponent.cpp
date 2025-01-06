#include "HealthComponent.h"
#include <string>
#include <iostream>

HealthComponent::HealthComponent(int maxHealth) : maxHP(maxHealth), currentHP(maxHealth) {}

void HealthComponent::initHeart() {
    if (!heartTexture.loadFromFile("resources/sprites/heart_animated_1.png")) {
        std::cerr << "Error loading heart texture!" << std::endl;
    }
    heartSprite.setTexture(heartTexture);
    heartSprite.setTextureRect(sf::IntRect(0,0,17,17));
    hurtAnimation = Animation(&heartTexture, sf::Vector2u(5, 1), 0.15f, sf::Vector2u(17, 17)); // Normal order
    healAnimation = Animation(&heartTexture, sf::Vector2u(5, 1), 0.15f, sf::Vector2u(17, 17), true); // Reverse order
}

void HealthComponent::takeDamage(int damage) {
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;
    playingHurtAnimation = true;
    animationTimer = 0.75f; 
}

void HealthComponent::heal(int amount) {
    currentHP += amount;
    if (currentHP > maxHP) currentHP = maxHP;
    playingHealAnimation = true;
    animationTimer = 0.75f; 
}

void HealthComponent::update(float deltaTime) {
    if (playingHurtAnimation) {
        animationTimer -= deltaTime;
        hurtAnimation.update(0, deltaTime); 
        if (animationTimer <= 0.0f) {
            playingHurtAnimation = false;
        }
    }

    if (playingHealAnimation) {
        animationTimer -= deltaTime;
        healAnimation.update(0, deltaTime); 
        if (animationTimer <= 0.0f) {
            playingHealAnimation = false;
        }
    }
} //FIXME : modify


void HealthComponent::renderHearts(sf::RenderTarget& target) {
    for (int i = 0; i < maxHP; ++i) {
        heartSprite.setPosition(10.f + i * 20.f, 10.f);
        heartSprite.setColor(i < currentHP ? sf::Color::White : sf::Color(128, 128, 128, 128));
        target.draw(heartSprite);

        if (playingHurtAnimation && i == currentHP) {
            sf::Sprite hurtSprite(*heartSprite.getTexture(), hurtAnimation.uvRect);
            hurtSprite.setPosition(heartSprite.getPosition());
            hurtSprite.setScale(0.5f, 0.5f);
            target.draw(hurtSprite);
        } else if (playingHealAnimation && i == currentHP) {
            sf::Sprite healSprite(*heartSprite.getTexture(), healAnimation.uvRect);
            healSprite.setPosition(heartSprite.getPosition());
            healSprite.setScale(0.5f, 0.5f);
            target.draw(healSprite);
        }
    }
}


int HealthComponent::getHealth() const {
    return currentHP;
}

int HealthComponent::getMaxHealth() const {
    return maxHP;
}

bool HealthComponent::isDead() const {
    return currentHP <= 0;
}

void HealthComponent::reset() {
    currentHP = maxHP;
}


//TODO: Implement texture opacity blinking when damage to the player