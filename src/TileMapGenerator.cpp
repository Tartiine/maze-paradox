#include "TileMapGenerator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

int TileMapGenerator::uniqueBatchCounter = 0;

vector<uint8_t> TileMapGenerator::saveTileMapToMemory(const vector<vector<int>>& tileMap) {
    vector<uint8_t> binaryData;
    for (const auto& row : tileMap) {
        for (const auto& tile : row) {
            binaryData.push_back(static_cast<uint8_t>(tile)); 
        }
    }

    return binaryData; 
}

void TileMapGenerator::saveTileMapToFile(const vector<vector<int>>& tileMap, const string& filename) {
    namespace fs = filesystem;
    fs::path filePath(filename);
    fs::path dirPath = filePath.parent_path();

    if (!fs::exists(dirPath)) {
        fs::create_directories(dirPath);
    }

    ofstream outFile(filePath, ios::binary | ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Failed to open file for writing: " << filePath << endl;
        return;
    }

    unsigned width = tileMap[0].size();
    unsigned height = tileMap.size();
    outFile.write(reinterpret_cast<const char*>(&width), sizeof(width));
    outFile.write(reinterpret_cast<const char*>(&height), sizeof(height));

    for (const auto& row : tileMap) {
        for (const auto& tile : row) {
            uint8_t tileValue = static_cast<uint8_t>(tile);
            outFile.write(reinterpret_cast<const char*>(&tileValue), sizeof(tileValue));
        }
    }

    outFile.close();
}

vector<vector<uint8_t>> TileMapGenerator::generateBatch(int batchSize, unsigned width, unsigned height, bool inMemory) {
    vector<vector<uint8_t>> tileMapsInMemory;
    for (int i = 0; i < batchSize; i++) {
        vector<vector<int>> tileMap = generateTileMap(width, height);
        if (!inMemory) {
            stringstream ss;
            ss << "resources/maps/batch_" << uniqueBatchCounter << "/tile_map_" << uniqueBatchCounter << "_" << i << ".bin";
            saveTileMapToFile(tileMap, ss.str());
        }

        vector<uint8_t> binaryTileMap = saveTileMapToMemory(tileMap);
        tileMapsInMemory.push_back(binaryTileMap);
    }

    uniqueBatchCounter++;
    return tileMapsInMemory;
}

vector<vector<uint8_t>> TileMapGenerator::generateStart() {
    vector<vector<int>> startingTileMapInt(22, vector<int>(40, 0));  
    for (int col = 0; col < 40; ++col) {
        startingTileMapInt[21][col] = 1;  
    }

    vector<uint8_t> binaryStartingTileMap = saveTileMapToMemory(startingTileMapInt);

    return {binaryStartingTileMap};
}

