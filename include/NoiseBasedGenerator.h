#ifndef NOISEBASED_GENERATOR_H
#define NOISEBASED_GENERATOR_H

#include "TileMapGenerator.h"
#include <vector>
#include <memory>

/**
 * NoiseBasedGenerator class generates maps using an intial noise
 * and a cellular automata to converge into a map.
 */

class NoiseBasedGenerator : public TileMapGenerator {
public:
    NoiseBasedGenerator();
    ~NoiseBasedGenerator() override;
    std::vector<std::vector<int>> generateTileMap(unsigned width, unsigned height) override;

private:
    void seedRandom();
    void initGrid();
    int countChain(int x, int y);
    int countStack(int x, int y);
    void update(unsigned int nbrOfGen);

    unsigned int row, col;
    std::unique_ptr<std::vector<std::vector<int>>> grid;
};

#endif

