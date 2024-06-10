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
    TileMap(unsigned width, unsigned height, float tileSize, const std::string& name);
    void render(sf::RenderTarget &target, bool debug = false);
    void loadMap(const std::string &fileName);

    std::unique_ptr<Obstacle>& getTile(unsigned row, unsigned col); 
    unsigned getWidth() const { return width; }  
    unsigned getHeight() const { return height; }  
    unsigned getTileSize() const { return tileSize; }  
    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f& newPosition);
    const std::string& getName() const;

private:
    std::vector<std::vector<std::unique_ptr<Obstacle>>> map;
    unsigned width;
    unsigned height;
    float tileSize;
    sf::Vector2f position;
    std::string name;

    void initMap();
    std::unique_ptr<Obstacle> createTile(int tileType, float x, float y);
};

#endif // TILEMAP_H


//TODO: Debug collisions