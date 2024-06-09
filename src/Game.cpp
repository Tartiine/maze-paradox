#include "Game.h"
#include "Platform.h"
#include "Ground.h"
#include <iostream>

Game::Game() : showGamepadFlag(true) {
    this->initWindow();
    initRenderTexture();
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
    this->player->checkWindowBorders(this->renderTexture);

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
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
            this->window.close();
            if (this->isFullscreenOn) {
                this->scale = 2;
                window.setMouseCursorVisible(true);
                this->initWindow();
            } else {
                window.setMouseCursorVisible(false);
                this->initWindowFullscreen();  
            }
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
            scale = (scale % 3) + 1;
            this->window.close();
            this->initWindow();
        }
    }

    this->updatePlayer(deltaTime);
    this->collisionPlayer();
}


void Game::renderPlayer(){
    this->player->render(this->renderTexture);
}

void Game::renderObstacles(bool debug) {
    tileMap->render(this->renderTexture, debug); 
}

void Game::render() {
    this->renderTexture.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components
    this->renderObstacles(true);
    this->renderPlayer();

    if (showGamepadFlag && infoClock.getElapsedTime().asSeconds() < 3) {
        this->renderTexture.draw(triangle);
    } else {
        showGamepadFlag = false;
    }

    renderTexture.display();

    sf::Sprite renderSprite(renderTexture.getTexture());
    renderSprite.setScale(this->scale, this->scale);
    renderSprite.setPosition(this->fullscreenHorizontalOffset, this->fullscreenVerticalOffset);
    
    this->window.clear();
    this->window.draw(renderSprite);
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
    this->window.create(sf::VideoMode(this->resolution.x * this->scale, this->resolution.y * this->scale), "SFML Platformer", sf::Style::Close | sf::Style::Titlebar);
    this->isFullscreenOn = false;
    this->fullscreenVerticalOffset = 0;
    this->fullscreenVerticalOffset = 0;
}

void Game::initWindowFullscreen() {
    sf::VideoMode fullScreen = sf::VideoMode::getFullscreenModes()[0];

    this->window.create(fullScreen, "SFML Platformer", sf::Style::Fullscreen);
    this->isFullscreenOn = true;
    this->scale = std::min(fullScreen.width / this->resolution.x, fullScreen.height / this->resolution.y);
    this->fullscreenHorizontalOffset = (fullScreen.width - (this->resolution.x * this->scale)) / 2;
    this->fullscreenVerticalOffset = (fullScreen.height - (this->resolution.y * this->scale)) / 2;
}

void Game::initRenderTexture() {
    this->renderTexture.create(this->resolution.x, this->resolution.y);
}

void Game::initPlayer() {
    this->player = new Player(64, 300);
}

void Game::initObstacles() {
    this->tileMap = new TileMap(16, 12, 50.0f);  
    this->tileMap->loadMap("resources/map.txt"); 
}
