#include "Game.h"
#include <iostream>

Game::Game() {
    this->initWindow();
    this->initPlatforms();
    this->initPlayer();
}

Game::~Game(){
    delete this->player;
    this->platforms.clear();
}

void Game::collisionPlayer() {
    for (const auto& platform : this->platforms) {
        sf::FloatRect bounds = platform->getBounds();
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

void Game::renderPlatforms(){
    for (auto platform : this->platforms) {
        platform->render(this->window);
    }
}

void Game::render() {
    this->window.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components
    this->renderPlatforms();
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

void Game::initPlatforms(){
    platforms.push_back(new Platform(0, 300));
    platforms.push_back(new Platform(48, 300));
    platforms.push_back(new Platform(48*2, 300));
    platforms.push_back(new Platform(48*3, 300));
    platforms.push_back(new Platform(48*4, 300));
    platforms.push_back(new Platform(48*5, 300));
    platforms.push_back(new Platform(48*6, 300));
    platforms.push_back(new Platform(48*7, 300));
    platforms.push_back(new Platform(48*8, 300));
    platforms.push_back(new Platform(48*9, 300));
    platforms.push_back(new Platform(48*10, 300));
    platforms.push_back(new Platform(48*11, 300));
    platforms.push_back(new Platform(48*12, 300));
    platforms.push_back(new Platform(48*13, 300));
    platforms.push_back(new Platform(48*14, 300));
    platforms.push_back(new Platform(48*15, 300));
}
