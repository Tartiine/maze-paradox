#include "Game.h"
#include "Platform.h"
#include "Ground.h"
#include <iostream>

Game::Game() : showGamepadFlag(true) {
    this->initWindow();
    this->initObstacles();
    this->initPlayer();

    checkGamepad();
}

Game::~Game() {
    delete this->player;
    delete this->tileMap;
    for (auto obstacle : this->obstacles) {
        delete obstacle;
    }
    this->obstacles.clear();
}

void Game::collisionPlayer() {
    this->player->checkWindowBorders(this->window); 
    for (unsigned i = 0; i < tileMap->getHeight(); ++i) {
        for (unsigned j = 0; j < tileMap->getWidth(); ++j) {
            auto& obstacle = tileMap->getTile(i, j);
            if (obstacle) {
                sf::FloatRect bounds = obstacle->getHitbox();
                if (this->player->isColliding(bounds)) {
                    player->resolveCollision(bounds);
                }
            }
        }
    }
}

void Game::updatePlayer(float deltaTime) {
    this->player->update(deltaTime);
}

void Game::update(float deltaTime) {
    sf::Event event;
    while (this->window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            this->window.close();
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            this->window.close();
        }
    }

    this->updatePlayer(deltaTime);
    this->collisionPlayer();
}

void Game::renderPlayer() {
    this->player->render(this->window);
}

void Game::renderObstacles(bool debug) {
    tileMap->render(this->window, debug); 
}

void Game::render() {
    this->window.clear(sf::Color::Blue);  // Maybe clear with a black color?

    // Drawing components
    this->renderObstacles(true);
    this->renderPlayer();

    if (showGamepadFlag && infoClock.getElapsedTime().asSeconds() < 3) {
        this->window.draw(triangle);
    } else {
        showGamepadFlag = false;
    }

    this->window.display();
}

const sf::RenderWindow& Game::getWindow() const {
    return this->window;
}

void Game::checkGamepad() {
    bool gamepadConnected = false;
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            gamepadConnected = true;
            break;
        }
    }
    createTriangle(gamepadConnected);
}

void Game::createTriangle(bool gamepadConnected) {
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(75.f, 10.f));
    triangle.setPoint(1, sf::Vector2f(100.f, 60.f));
    triangle.setPoint(2, sf::Vector2f(50.f, 60.f));
    if (gamepadConnected) {
        triangle.setFillColor(sf::Color::Green);
    } else {
        triangle.setFillColor(sf::Color::Red);
    }
}

void Game::initWindow() {
    this->window.create(sf::VideoMode(800, 600), "SFML Platformer", sf::Style::Close | sf::Style::Titlebar);
    this->window.setFramerateLimit(60);  // TODO: Uniformize the movement speed (no variation based on FPS)
}

void Game::initPlayer() {
    this->player = new Player();
}

void Game::initObstacles() {
    this->tileMap = new TileMap(16, 12, 50.0f);  
    this->tileMap->loadMap("resources/map.txt"); 
}
