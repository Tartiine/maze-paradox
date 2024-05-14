#include "Game.h"

Game::Game() {
    this->initWindow();
    this->initPlatform();
    this->initPlayer();
}

Game::~Game(){
    delete this->player;
    delete this->platform;
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
}


void Game::renderPlayer(){
    this->player->render(this->window);
}

void Game::renderPlatform(){
    this->platform->render(this->window);
}

void Game::render() {
    this->window.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components
    this->renderPlatform();
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

void Game::initPlatform(){
    this->platform = new Platform();
}
