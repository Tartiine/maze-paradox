#include "TileMapGenerator.h"
#include "RuleBasedGenerator.h"
#include "NoiseBasedGenerator.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void TileMapGenerator::saveTileMapToFile(const std::vector<std::vector<int>>& tileMap, const std::string& filename) {
    ofstream outFile(filename, ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Failed to open file for writing: " << filename << endl;
        return;
    }

    for (const auto& row : tileMap) {
        for (const auto& tile : row) {
            outFile << tile << ' ';
        }
        outFile << '\n';
    }

    outFile.close();
}

void TileMapGenerator::generateBatch(int batchSize, unsigned width, unsigned height, const std::string& filePrefix) {
    for (int i = 0; i < batchSize; i++) {
        std::vector<std::vector<int>> tileMap = generateTileMap(width, height);
        std::stringstream filename;
        filename << filePrefix << i << ".txt";
        saveTileMapToFile(tileMap, filename.str());
    }
}