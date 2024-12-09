#ifndef TILE_MAP_GENERATOR_H
#define TILE_MAP_GENERATOR_H

#include <vector>
#include <string>
#include <cstdint>

/**
 * TileMapGenerator class is an abstract class that allows to automatically create maps
 * as well as saving them, and creating batches for the NN learning.
 */

class TileMapGenerator {
public:
    virtual std::vector<std::vector<int>> generateTileMap(unsigned width, unsigned height) = 0;
    std::vector<uint8_t> saveTileMapToMemory(const std::vector<std::vector<int>>& tileMap);
    void saveTileMapToFile(const std::vector<std::vector<int>> &tileMap, const std::string &filename);
    std::vector<std::vector<uint8_t>> generateBatch(int batchSize, unsigned width, unsigned height, bool inMemory);
    std::vector<std::vector<uint8_t>> generateStart();
    virtual ~TileMapGenerator() = default;
private :
    static int uniqueBatchCounter;
};

#endif //TILE_MAP_GENERATOR_H

