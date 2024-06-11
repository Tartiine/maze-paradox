#ifndef NOISEBASED_GENERATOR_H
#define NOISEBASED_GENERATOR_H

#include "TileMapGenerator.h"
#include <vector>
#include <string>

class NoiseBasedGenerator : public TileMapGenerator {
public:
    NoiseBasedGenerator();
    ~NoiseBasedGenerator();
    std::vector<std::vector<int>> generateTileMap(unsigned width, unsigned height) override;

private:
    void seedRandom();
    void initGrid();
    int countChain(int x, int y);
    int countStack(int x, int y);
    void update(unsigned int nbrOfGen);

    unsigned int row, col;
    std::vector<std::vector<int>> grid;
};

#endif

