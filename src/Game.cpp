#include "Game.h"
#include "Platform.h"
#include "Ground.h"
#include <iostream>

Game::Game() {
    this->initWindow();
    initRenderTexture();
    this->initObstacles();
    this->initPlayer();
}

Game::~Game(){
    delete this->player;
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
    this->player->render(this->renderTexture);
}

void Game::renderObstacles(){
    for (auto obstacle : this->obstacles) {
        obstacle->render(this->renderTexture);
    }
}

void Game::render() {
    this->renderTexture.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components
    this->renderObstacles();
    this->renderPlayer();

    renderTexture.display();

    sf::Sprite renderSprite(renderTexture.getTexture());
    renderSprite.setScale(this->scale, this->scale);
    
    this->window.clear();
    this->window.draw(renderSprite);
    this->window.display();  
}

const sf::RenderWindow & Game::getWindow() const {
    return this->window;
}

void Game::initWindow(){
    this->window.create(sf::VideoMode(this->resolution.x * this->scale, this->resolution.y * this->scale), "SFML Platformer", sf::Style::Close | sf::Style::Titlebar);
    this->window.setFramerateLimit(60);  //TODO: uniformiser la vitesse de deplacement (pas de variation en fonction des fps)
}

void Game::initRenderTexture() {
    this->renderTexture.create(this->resolution.x, this->resolution.y);
}

void Game::initPlayer(){
    this->player = new Player(64, 0);
}

void Game::initObstacles(){
    for (int i = 2; i < 18; ++i) {
        obstacles.push_back(new Ground(16*i, 164));
    }
}