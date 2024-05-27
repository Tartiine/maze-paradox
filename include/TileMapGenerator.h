#ifndef TILEMAPGENERATOR_H
#define TILEMAPGENERATOR_H

#include <vector>
#include <string>

using namespace std;

class TileMapGenerator {
public:
    vector<vector<int>> generateTileMap(unsigned width, unsigned height);
    void saveTileMapToFile(const vector<vector<int>>& tileMap, const string& filename);
};

#endif // TILEMAPGENERATOR_H
