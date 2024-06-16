#include "TileMapGenerator.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

using namespace std;

void TileMapGenerator::saveTileMapToFile(const vector<vector<int>>& tileMap, const string& filename) {
    namespace fs = filesystem;
    fs::path filePath = filename;
    fs::path dirPath = filePath.parent_path();

    if (!fs::exists(dirPath)) {
        if (!fs::create_directories(dirPath)) {
            cerr << "Failed to create directory: " << dirPath << endl;
            return;
        }
    }

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

void TileMapGenerator::generateBatch(int batchSize, unsigned width, unsigned height, const string& filePrefix) {
    for (int i = 0; i < batchSize; i++) {
        vector<vector<int>> tileMap = generateTileMap(width, height);
        stringstream filename;
        filename << filePrefix << i << ".txt";
        saveTileMapToFile(tileMap, filename.str());
    }
}