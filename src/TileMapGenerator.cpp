#include "TileMapGenerator.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <utility>
#include <iostream>

using namespace std;

TileMapGenerator::TileMapGenerator() {
}

float TileMapGenerator::degreesToRadians(float degrees) {
    return degrees * M_PI / 180.0;
}

vector<vector<int>> TileMapGenerator::generateTileMap(unsigned width, unsigned height) {
    vector<vector<int>> tileMap(height, vector<int>(width, 0));
    
    srand(static_cast<unsigned>(time(0)));
    const unsigned maxTiles = 20;
    const float minDist = 2.0f;
    const float maxDist = 3.0f;
    unsigned tileCount = 0;

    auto distance = [](int x1, int y1, int x2, int y2) {
        return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    };

    auto direction = [](int x1, int y1, int x2, int y2) {
        return make_pair(x2 - x1, y2 - y1);
    };

    auto isPointInPolygon = [](int px, int py, const vector<pair<int, int>>& polygon) {
        int n = polygon.size();
        int intersections = 0;

        for (int i = 0; i < n; ++i) {
            auto [x1, y1] = polygon[i];
            auto [x2, y2] = polygon[(i + 1) % n];

            if (((y1 > py) != (y2 > py)) &&
                (px < (x2 - x1) * (py - y1) / (y2 - y1) + x1)) {
                intersections++;
            }
        }

        return (intersections % 2 != 0);
    };

    vector<pair<int, int>> tilePositions;

    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width && tileCount < maxTiles; ++j) {
            bool canPlace = true;
            bool mustPlace = false;
            for (const auto& pos : tilePositions) {
                float dist = distance(j, i, pos.first, pos.second); 
                if (dist < minDist) {
                    canPlace = false;
                    break;
                } else if (dist >= maxDist) { 
                    auto dir = direction(pos.first, pos.second, j, i); 
                    float angle = atan2(dir.second, dir.first);

                    if (angle < -M_PI) {
                        angle += 2 * M_PI;
                    } else if (angle > M_PI) {
                        angle -= 2 * M_PI;
                    }

                    if (angle >= -M_PI / 3 && angle <= -M_PI / 6) {
                        vector<pair<int, int>> polygon = {{j, i},
                            {j + maxDist * cos(degreesToRadians(-170)), i + maxDist * sin(degreesToRadians(-170))},
                            {j + maxDist * cos(degreesToRadians(-135)), i + maxDist * sin(degreesToRadians(-135))},
                            {j + maxDist * cos(degreesToRadians(-45)), i + maxDist * sin(degreesToRadians(-45))},
                            {j + maxDist * cos(degreesToRadians(-10)), i + maxDist * sin(degreesToRadians(-10))}
                        };
                        for (const auto& tilePos : tilePositions) {
                            if (isPointInPolygon(tilePos.first, tilePos.second, polygon)) {
                                //mustPlace = true;
                                break;
                            }
                        }
                    }
                }
            }
            if (canPlace && i != 0) {
                int tileType;
                if(mustPlace){
                    tileType = rand() % 2 + 1; 
                } else {
                    int randomValue = rand() % 100;  
                    if (randomValue < 7) {
                        tileType = 2;  // 5% chance
                    } else if (randomValue < 30) {
                        tileType = 1;  // 25% chance 
                    } else {
                        tileType = 0;  // 60% chance 
                    }
                }
                if (tileType == 1 && j + 1 < width) {
                    tileMap[i][j] = tileType;
                    tileMap[i][j + 1] = tileType;
                    tilePositions.emplace_back(j, i);
                    tilePositions.emplace_back(j + 1, i);
                    tileCount++;
                    j++; 
                } else if (tileType == 2) {
                    tileMap[i][j] = tileType;
                    tilePositions.emplace_back(j, i);
                    tileCount++;
                }
            }
        }
    }
    // Always place ground tiles on the bottom of the screen
    for (unsigned j = 0; j < width; ++j) {
        tileMap[height - 1][j] = 2;
    } 

    return tileMap;
}

    /*TODO: Add more rules: 
    - Maximum number of tiles per tileMap
    - Max and min distance between tiles in diagonal, height and width, add obligatory tile when last chance for max dist !
    - No number 1 close to the borders
    */


void TileMapGenerator::saveTileMapToFile(const vector<vector<int>>& tileMap, const string& filename) {
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

