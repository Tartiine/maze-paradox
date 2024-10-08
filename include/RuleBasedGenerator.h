#ifndef RULEBASED_GENERATOR_H
#define RULEBASED_GENERATOR_H

#include "TileMapGenerator.h"
#include <vector>
#include <string>

/**
 * RuleBasedGenerator class generates maps using an intial noise
 * and then applying a specific set of rules to create a map.
 */

class RuleBasedGenerator : public TileMapGenerator {
public:
    RuleBasedGenerator();
    virtual ~RuleBasedGenerator();
    std::vector<std::vector<int>> generateTileMap(unsigned width, unsigned height);

private : 
    float degreesToRadians(float degrees);
};

#endif // RULEBASED_GENERATOR_H
