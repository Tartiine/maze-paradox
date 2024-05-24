#include "Game.h"
#include "Platform.h"
#include "Ground.h"
#include <iostream>

Game::Game() : showGamepadMessageFlag(true) {
    this->initWindow();
    initRenderTexture();
    this->initObstacles();
    this->initPlayer();

    checkGamepad();
}

Game::~Game() {
    delete this->player;
    for (auto obstacle : this->obstacles) {
        delete obstacle;
    }
    this->obstacles.clear();
}

void Game::collisionPlayer() {
    this->player->checkWindowBorders(this->renderTexture);

    for (const auto& platform : this->obstacles) {
        sf::FloatRect bounds = platform->getHitbox();
        if (this->player->isColliding(bounds)) {
            player->resolveCollision(bounds);
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
                this->initWindow();
                window.setMouseCursorVisible(true);
            } else {
                this->initWindowFullscreen();
                window.setMouseCursorVisible(false);
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

void Game::renderObstacles() {
    for (auto obstacle : this->obstacles) {
        obstacle->render(this->renderTexture);
    }
}

void Game::render() {
    this->renderTexture.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components
    this->renderObstacles();
    this->renderPlayer();

    // Display gamepad message for the first 3 seconds
    if (showGamepadMessageFlag && messageClock.getElapsedTime().asSeconds() < 3) {
        this->renderTexture.draw(triangle);
    } else {
        showGamepadMessageFlag = false;
    }

    renderTexture.display();

    sf::Sprite renderSprite(renderTexture.getTexture());
    renderSprite.setScale(this->scale, this->scale);
    
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
}

void Game::initWindowFullscreen() {
    this->window.create(sf::VideoMode::getFullscreenModes()[0], "SFML Platformer", sf::Style::Fullscreen);
    this->isFullscreenOn = true;
}

void Game::initRenderTexture() {
    this->renderTexture.create(this->resolution.x, this->resolution.y);
}

void Game::initPlayer() {
    this->player = new Player(64, 320);
}

void Game::initObstacles(){
    // 40 by 22.5 grid
    for (int i = 2; i < 38; ++i) {
        obstacles.push_back(new Ground(16*i, 344));
    }
}
