#include "Game.h"
#include "Platform.h"
#include "Ground.h"
#include <iostream>

Game::Game() {
    this->initWindow();
    this->initObstacles();
    this->initPlayer();
}

Game::~Game(){
    delete this->player;
    this->obstacles.clear();
}

void Game::collisionPlayer() {
    for (const auto& platform : this->obstacles) {
        sf::FloatRect bounds = platform->getHitbox();
        if (this->player->isColliding(bounds)) {
            player->resolveCollision(bounds);
        }
    }
}

void Game::updatePlayer(float deltaTime){
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


void Game::renderPlayer(){
    this->player->render(this->window);
}

void Game::renderObstacles(){
    for (auto obstacle : this->obstacles) {
        obstacle->render(this->window);
    }
}

void Game::render() {
    this->window.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components
    this->renderObstacles();
    this->renderPlayer();

    this->window.display();  
}

const sf::RenderWindow & Game::getWindow() const {
    return this->window;
}

void Game::initWindow(){
    this->window.create(sf::VideoMode(800, 600), "SFML Platformer", sf::Style::Close | sf::Style::Titlebar);
    this->window.setFramerateLimit(60);  //TODO: uniformiser la vitesse de deplacement (pas de variation en fonction des fps)
}

void Game::initPlayer(){
    this->player = new Player();
}

void Game::initObstacles(){
    for (int i = 1; i < 15; ++i) {
        obstacles.push_back(new Ground(48*i, 400));
    }

    obstacles.push_back(new Platform(200, 300));
    obstacles.push_back(new Platform(400, 300));
    obstacles.push_back(new Platform(600, 300));  
}