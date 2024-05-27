#include "TileMap.h"
#include <fstream>
#include <sstream>
#include <iostream>

TileMap::TileMap(unsigned width, unsigned height, float tileSize)
    : width(width), height(height), tileSize(tileSize) {
    initMap();
}

void TileMap::initMap() {
    map.resize(height);
    for (auto &row : map) {
        row.resize(width);
    }
}

std::unique_ptr<Obstacle> TileMap::createTile(int tileType, float x, float y) {
    std::unique_ptr<Obstacle> tile = nullptr;
    switch (tileType) {
        case 1:
            tile = std::make_unique<Platform>(x, y);
            break;
        case 2:
            tile = std::make_unique<Ground>(x, y);
            break;
        default:
            break;
    }
    return tile;
}

void TileMap::render(sf::RenderTarget &target, bool debug) {
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            if (map[i][j]) {
                map[i][j]->render(target);
            }
        }
    }

    if (debug) {
        sf::VertexArray grid(sf::Lines);
        for (unsigned i = 0; i <= height; ++i) {
            grid.append(sf::Vertex(sf::Vector2f(0, i * tileSize), sf::Color::Red));
            grid.append(sf::Vertex(sf::Vector2f(width * tileSize, i * tileSize), sf::Color::Red));
        }
        for (unsigned j = 0; j <= width; ++j) {
            grid.append(sf::Vertex(sf::Vector2f(j * tileSize, 0), sf::Color::Red));
            grid.append(sf::Vertex(sf::Vector2f(j * tileSize, height * tileSize), sf::Color::Red));
        }
        target.draw(grid);
    }
}

void TileMap::loadMap(const std::string &fileName) {
    std::ifstream inFile(fileName);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open map file: " << fileName << std::endl;
        return;
    }

    std::string line;
    for (unsigned i = 0; i < height; ++i) {
        std::getline(inFile, line);
        std::istringstream ss(line);
        for (unsigned j = 0; j < width; ++j) {
            int tileType;
            ss >> tileType;
            if (tileType == 1) {
                if (j + 1 < width) {
                    int nextTileType;
                    ss >> nextTileType;
                    if (nextTileType == 1) {
                        map[i][j] = createTile(tileType, j * tileSize, i * tileSize);
                        map[i][j + 1] = nullptr; 
                        j++; 
                    }
                }
            } else if (tileType == 2) {
                map[i][j] = createTile(tileType, j * tileSize, i * tileSize);
            } else {
                map[i][j] = nullptr;
            }
        }
    }

    inFile.close();
}

std::unique_ptr<Obstacle>& TileMap::getTile(unsigned row, unsigned col) {
    return map[row][col];
}
