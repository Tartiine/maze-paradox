#ifndef WORLD_STATE_H
#define WORLD_STATE_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "EnemyAI.h"
#include "TileMapManager.h"


struct WorldState {
    // Player-related state
    sf::Vector2f playerPosition;
    sf::Vector2f playerVelocity;
    HealthComponent playerHealth;
    bool playerHasJumped;
    Player::State playerState;
    bool playerPhysicsChanged;
    float playerPhysicsEffectRemainingTime;

    // EnemyAI-related state
    sf::Vector2f enemyPosition;
    int enemyJumpCount;
    float enemyJumpTimer;
    bool enemyAggressiveMode;

    // TileMap-related state
    sf::Vector2i tileToDelete;

    // Methods
    WorldState();
    WorldState(const WorldState& other);
    ~WorldState() = default;

    void updateFromPlayer(const Player& player);
    void updateFromEnemyAI(const EnemyAI& enemyAI);
    void updateFromTileMapManager(const TileMapManager& manager);
};

#endif // WORLD_STATE_H