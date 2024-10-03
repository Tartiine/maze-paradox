#include "NoiseBasedGenerator.h"
#include <ctime>
#include <cstdlib>

using namespace std;

NoiseBasedGenerator::NoiseBasedGenerator() : row(0), col(0) { 
    seedRandom();
}

NoiseBasedGenerator::~NoiseBasedGenerator() {}

void NoiseBasedGenerator::seedRandom() {
    srand(static_cast<unsigned>(time(0)));
}

void NoiseBasedGenerator::initGrid() {
    for (unsigned i = 0; i < row; ++i) {
        for (unsigned j = 0; j < col; ++j) {
            int type = (rand() % 2) * 2;
            (*grid)[i][j] = type; 
        }
    }
}

int NoiseBasedGenerator::countChain(int x, int y) {
    int count = 1;

    // Counts connected blocs on the left
    for (int i = 1; i < col; ++i) {
        if (y-i >= 0 && (*grid)[x][y-i] == (*grid)[x][y]) { 
            count++;
        } else {
            break;
        }
    }

    // Counts connected blocs on the right
    for (int i = 1; i < col; ++i) {
        if (y+i < col && (*grid)[x][y+i] == (*grid)[x][y]) { 
            count++;
        } else {
            break;
        }
    }

    return count;
}

int NoiseBasedGenerator::countStack(int x, int y) {
    int count = 1;

    // Counts connected blocs on the top
    for (int i = 1; i < row; ++i) {
        if (x-i >= 0 && (*grid)[x-i][y] == (*grid)[x][y]) { 
            count++;
        } else {
            break;
        }
    }

    // Counts connected blocs on the bottom
    for (int i = 1; i < row; ++i) {
        if (x+i < row && (*grid)[x+i][y] == (*grid)[x][y]) { 
            count++;
        } else {
            break;
        }
    }

    return count;
}

void NoiseBasedGenerator::update(unsigned int nbrOfGen) {
    for (int gen = 0; gen < nbrOfGen; ++gen) {
        auto newGrid = make_unique<vector<vector<int>>>(*grid); 
        
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                int chain = countChain(i, j);
                int stack = countStack(i, j);
                
                if ((*grid)[i][j] == 2 && stack > 3) { 
                    (*newGrid)[i][j] = 0;
                } else if ((*grid)[i][j] == 2 && chain > 1) { 
                    (*newGrid)[i][j] = 2;
                } else {
                    (*newGrid)[i][j] = 0;
                }
            }
        }
        grid = move(newGrid); 
    }
}

vector<vector<int>> NoiseBasedGenerator::generateTileMap(unsigned width, unsigned height) {
    row = height;
    col = width;
    grid = make_unique<vector<vector<int>>>(row, vector<int>(col, 0));
    initGrid();
    unsigned int nbrOfGen = 10; 
    update(nbrOfGen);
    return *grid;
}

