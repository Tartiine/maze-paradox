#include "Game.h"

Game::Game() {
    this->initWindow();
    this->initPlayer();
}

Game::~Game(){
    delete this->player;
}

void Game::updatePlayer(){
    this->player->update();

}

void Game::update()
{
    sf::Event event;
    while (this->window.pollEvent(this->ev)) {
        if (this->ev.type == sf::Event::Closed) {
            this->window.close();
        }else if (this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Escape) {
            this->window.close();
        }
    }

    this->updatePlayer();
}

void Game::renderPlayer(){
    this->player->render(this->window);
}

void Game::render() {
    this->window.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components
    this->renderPlayer();

    this->window.display();  
}

const sf::RenderWindow & Game::getWindow() const {
    return this->window;
}

void Game::initWindow(){
    this->window.create(sf::VideoMode(800, 600), "SFML Platformer", sf::Style::Close | sf::Style::Titlebar);
    this->window.setFramerateLimit(144);  
}

void Game::initPlayer(){
    this->player = new Player();
}
