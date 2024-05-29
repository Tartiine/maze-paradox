#include "TileMapGeneratorCA.h"

TileMapGeneratorCA::TileMapGeneratorCA(unsigned int _col, unsigned int _row) : row(_row), col(_col), grid(row, vector<int>(col, 0)) {
    this->initGrid();
}

TileMapGeneratorCA::~TileMapGeneratorCA() {}

void TileMapGeneratorCA::initGrid() {
    srand(static_cast<unsigned>(time(0)));

    for (unsigned i = 0; i < row; ++i) {
        for (unsigned j = 0; j < col; ++j) {
            int type = (rand() % 2) * 2;
            grid[i][j] = type;
        }
    }
}

int TileMapGeneratorCA::countChain(int x, int y) {
    int count = 1;

    // Counts connected blocs on the left
    for (int i = 1; i < col; ++i) {
        if (y-i >= 0 && grid[x][y-i] == grid[x][y]) {
            count++;
        } else {
            break;
        }
    }

    // Counts connected blocs on the right
    for (int i = 1; i < col; ++i) {
        if (y+i < col && grid[x][y+i] == grid[x][y]) {
            count++;
        } else {
            break;
        }
    }

    return count;
}

int TileMapGeneratorCA::countStack(int x, int y) {
    int count = 1;

    // Counts connected blocs on the top
    for (int i = 1; i < row; ++i) {
        if (x-i >= 0 && grid[x-i][y] == grid[x][y]) {
            count++;
        } else {
            break;
        }
    }

    // Counts connected blocs on the bottom
    for (int i = 1; i < row; ++i) {
        if (x+i < row && grid[x+i][y] == grid[x][y]) {
            count++;
        } else {
            break;
        }
    }

    return count;
}

void TileMapGeneratorCA::update(unsigned int nbrOfGen) {
    for (int gen = 0; gen < nbrOfGen; ++gen) {
        vector<vector<int>> newGrid = grid;
        
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                int chain = countChain(i, j);
                int stack = countStack(i, j);
                
                if (grid[i][j] == 2 && stack > 3) {
                    newGrid[i][j] = 0;
                } else if (grid[i][j] == 2 && chain > 1) {
                    newGrid[i][j] = 2;
                } else {
                    newGrid[i][j] = 0;
                }
            }
        }

        grid = newGrid;
        
        string filename = "resources/temp" + to_string(gen) + ".txt";
        saveTileMapToFile(filename);
    }
}

void TileMapGeneratorCA::saveTileMapToFile(const string& filename) {
    ofstream outFile(filename, ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cerr << "Failed to open file for writing: " << filename << endl;
        return;
    }

    for (const auto& row : grid) {
        for (const auto& tile : row) {
            outFile << tile << ' ';
        }
        outFile << '\n';
    }

    outFile.close();
}