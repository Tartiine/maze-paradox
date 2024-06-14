#include "RuleBasedGenerator.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <utility>
#include <iostream>
#include <random>

using namespace std;

RuleBasedGenerator::RuleBasedGenerator() {
}

float RuleBasedGenerator::degreesToRadians(float degrees) {
    return degrees * M_PI / 180.0;
}

vector<vector<int>> RuleBasedGenerator::generateTileMap(unsigned width, unsigned height) {
    vector<vector<int>> tileMap(height, vector<int>(width, 0));

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 99);
        const unsigned maxTiles = 55;
        const float minDist = 3.f;
        const float maxDist = 100.f;
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

        for (unsigned i = 1; i < height - 1; ++i) {
            for (unsigned j = 0; j < width && tileCount < maxTiles; ++j) {
                bool canPlace = true;
                bool mustPlace = false;
                for (const auto& pos : tilePositions) {
                    float dist = distance(j, i, pos.first, pos.second);
                    if (dist < minDist) {
                        canPlace = false;
                        break;
                    } else if (dist >= maxDist && dist <= maxDist + 1.f) {
                        auto dir = direction(pos.first, pos.second, j, i);
                        float angle = atan2(dir.second, dir.first);

                        if (angle >= 0.5 && angle <= 1) {
                            vector<pair<int, int>> polygon = {
                                {pos.first + maxDist * cos(degreesToRadians(170)), pos.second + maxDist * sin(degreesToRadians(170))},
                                {pos.first + maxDist * cos(degreesToRadians(135)), pos.second + maxDist * sin(degreesToRadians(135))},
                                {pos.first + maxDist * cos(degreesToRadians(45)), pos.second + maxDist * sin(degreesToRadians(45))},
                                {pos.first + maxDist * cos(degreesToRadians(10)), pos.second + maxDist * sin(degreesToRadians(10))}
                            };

                            mustPlace = true;
                            for (const auto& tilePos : tilePositions) {
                                if (isPointInPolygon(tilePos.first, tilePos.second, polygon)) {
                                    mustPlace = false;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (canPlace) {
                    int tileType;
                    if (mustPlace) {
                        tileType = dis(gen) % 2 + 1;
                    } else {
                        int randomValue = dis(gen);
                        if (randomValue <= 10) {
                            tileType = 2;
                        } else if (randomValue < 30 && randomValue > 10) {
                            tileType = 1;
                        } else {
                            tileType = 0;
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

        return tileMap;
}

    /*TODO: Add more rules: 
    - Add max platforms per line
    - No number 1 close to the borders
    */

