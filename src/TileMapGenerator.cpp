#include "TileMapGenerator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;

vector<uint8_t> TileMapGenerator::saveTileMapToMemory(const vector<vector<int>>& tileMap) {
    vector<uint8_t> binaryData;
    for (const auto& row : tileMap) {
        for (const auto& tile : row) {
            binaryData.push_back(static_cast<uint8_t>(tile)); 
        }
    }

    return binaryData; 
}


vector<vector<uint8_t>> TileMapGenerator::generateBatch(int batchSize, unsigned width, unsigned height) {
    vector<vector<uint8_t>> tileMapsInMemory;
    for (int i = 0; i < batchSize; i++) {
        vector<vector<int>> tileMap = generateTileMap(width, height);
        vector<uint8_t> binaryTileMap = saveTileMapToMemory(tileMap);
        tileMapsInMemory.push_back(binaryTileMap);
    }
    return tileMapsInMemory;
}