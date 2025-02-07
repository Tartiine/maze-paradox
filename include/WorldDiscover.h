#ifndef WORLD_DISCOVER_H
#define WORLD_DISCOVER_H

#include "WorldModel.h"
#include "Player.h"
#include "EnemyAI.h"
#include "TileMapManager.h"

class WorldDiscover {
    public:
        WorldDiscover(const Player& player, const EnemyAI& enemyAI, const TileMapManager& tileMapManager);
        ~WorldDiscover() = default;

        void updateFromPlayer(WorldModel& model);
        void updateFromEnemyAI(WorldModel& model);
        void updateFromTileMapManager(WorldModel& model);

        void update(WorldModel& model);

    private:
        Player player;
        EnemyAI enemyAI;
        TileMapManager tileMapManager;
};

#endif // WORLD_DISCOVER_H