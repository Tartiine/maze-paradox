#ifndef TILE_MAP_GENERATOR_H
#define TILE_MAP_GENERATOR_H

#include <vector>
#include <string>

class TileMapGenerator {
public:
    virtual std::vector<std::vector<int>> generateTileMap(unsigned width, unsigned height) = 0;
    void saveTileMapToFile(const std::vector<std::vector<int>>& tileMap, const std::string& filename);
    virtual ~TileMapGenerator() = default;
    void generateBatch(int batchSize, unsigned width, unsigned height, const std::string& filePrefix);
};

#endif //TILE_MAP_GENERATOR_H
