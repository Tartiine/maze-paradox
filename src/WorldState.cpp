#include "WorldState.h"

WorldState::WorldState()
    : playerPosition(0.f, 0.f),
    playerVelocity(0.f, 0.f),
    playerHealth(0),
    playerHasJumped(false),
    playerState(Player::State::Idle),
    playerPhysicsChanged(false),
    playerPhysicsEffectRemainingTime(0.f),
    enemyPosition(0.f, 0.f),
    enemyJumpCount(0),
    enemyJumpTimer(0.f),
    enemyAggressiveMode(false),
    tileToDelete(0, 0) {}

WorldState::WorldState(const WorldState& other)
    : playerPosition(other.playerPosition),
      playerVelocity(other.playerVelocity),
      playerHealth(other.playerHealth),
      playerHasJumped(other.playerHasJumped),
      playerState(other.playerState),
      playerPhysicsChanged(other.playerPhysicsChanged),
      playerPhysicsEffectRemainingTime(other.playerPhysicsEffectRemainingTime),
      enemyPosition(other.enemyPosition),
      enemyJumpCount(other.enemyJumpCount),
      enemyJumpTimer(other.enemyJumpTimer),
      enemyAggressiveMode(other.enemyAggressiveMode),
      tileToDelete(other.tileToDelete) {}

void WorldState::updateFromPlayer(const Player &player)
{
    playerPosition = player.getPosition();
    playerVelocity = player.getVelocity();
    playerHealth = player.getHealth();
    playerHasJumped = player.hasJumped();
    playerState = player.getCurrentState();
    playerPhysicsChanged = player.isPhysicsChanged();
    playerPhysicsEffectRemainingTime = player.getPhysicsEffectRemainingTime();
}
void WorldState::updateFromEnemyAI(const EnemyAI &enemyAI)
{
    enemyPosition = enemyAI.getPosition();
    enemyJumpCount = enemyAI.getJumpCount();
    enemyJumpTimer = enemyAI.getJumpTimer();
    enemyAggressiveMode = enemyAI.isInAggressiveMode();
}

void WorldState::updateFromTileMapManager(const TileMapManager &manager)
{
    tileToDelete = manager.getTileToDelete();
}