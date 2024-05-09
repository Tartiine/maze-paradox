#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 600), "SFML Platformer") {
    window.setFramerateLimit(60);  
}

void Game::update() {
    sf::Event event;
    while (this->window.pollEvent(this->ev)) {
        if (this->ev.type == sf::Event::Closed) {
            this->window.close();
        }else if (this->ev.type == sf::Event::KeyPressed && this->ev.key.code == sf::Keyboard::Escape) {
            this->window.close();
        }
    }

}


void Game::render() {
    this->window.clear(sf::Color::Blue);  //Maybe clear with a black color ?

    // Drawing components

    this->window.display();  
}

const sf::RenderWindow & Game::getWindow() const {
    return this->window;
}
