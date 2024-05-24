#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Obstacle.h"
#include "Platform.h"
#include "Ground.h"

class TileMap {
public:
    TileMap(unsigned width, unsigned height, float tileSize);
    void render(sf::RenderTarget &target, bool debug = false);
    void loadMap(const std::string &fileName);

    std::unique_ptr<Obstacle>& getTile(unsigned row, unsigned col); 
    unsigned getWidth() const { return width; }  
    unsigned getHeight() const { return height; }  

private:
    std::vector<std::vector<std::unique_ptr<Obstacle>>> map;
    unsigned width;
    unsigned height;
    float tileSize;

    void initMap();
    std::unique_ptr<Obstacle> createTile(int tileType, float x, float y);
};

#endif // TILEMAP_H


//TODO: Debug collision chute