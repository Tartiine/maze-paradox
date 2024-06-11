#ifndef RULEBASED_GENERATOR_H
#define RULEBASED_GENERATOR_H

#include "TileMapGenerator.h"
#include <vector>
#include <string>

using namespace std;

class RuleBasedGenerator : public TileMapGenerator {
public:
    RuleBasedGenerator();
    //TODO: Add destructor
    vector<vector<int>> generateTileMap(unsigned width, unsigned height);

private : 
    float degreesToRadians(float degrees);
};

#endif // RULEBASED_GENERATOR_H
