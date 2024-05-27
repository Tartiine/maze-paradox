#include "TileMapGenerator.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

vector<vector<int>> TileMapGenerator::generateTileMap(unsigned width, unsigned height) {
    vector<vector<int>> tileMap(height, vector<int>(width, 0));

    srand(static_cast<unsigned>(time(0)));
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            int tileType = rand() % 3;
            tileMap[i][j] = tileType;
            if (tileType == 1){
                tileMap[i+1][j] = tileType;
                i++;
            }
        }
    }
    // Always place ground tiles on the bottom of the screen
    for (unsigned j = 0; j < width; ++j) {
        tileMap[height - 1][j] = 2;
    }

    return tileMap;
}

void TileMapGenerator::saveTileMapToFile(const vector<vector<int>>& tileMap, const string& filename) {
    ofstream outFile(filename);
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
