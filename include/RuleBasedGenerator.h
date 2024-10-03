#ifndef RULEBASED_GENERATOR_H
#define RULEBASED_GENERATOR_H

#include "TileMapGenerator.h"
#include <vector>
#include <string>

class RuleBasedGenerator : public TileMapGenerator {
public:
    RuleBasedGenerator();
    virtual ~RuleBasedGenerator();
    std::vector<std::vector<int>> generateTileMap(unsigned width, unsigned height);

private : 
    float degreesToRadians(float degrees);
};

#endif // RULEBASED_GENERATOR_H
