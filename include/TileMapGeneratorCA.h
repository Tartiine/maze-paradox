#ifndef TILEMAPGENERATOR_CA_H
#define TILEMAPGENERATOR_CA_H

#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;

class TileMapGeneratorCA {
    public:
        TileMapGeneratorCA(unsigned int _col, unsigned int _row);
        ~TileMapGeneratorCA();

        void seedRandom();

        void initGrid();

        void update(unsigned int nbrOfGen);

        void saveTileMapToFile(const string &filename);

    private:
        int countChain(int x, int y);
        int countStack(int x, int y);

        unsigned int row, col;
        vector<vector<int>> grid;

        unsigned int minBlocPlat = 2;
};

#endif // TILEMAPGENERATOR_CA_H