#include "Game.h"

int main() {
    Game game;
    sf::Clock clock;

    while (game.getWindow().isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        game.update(deltaTime);
        game.render();
    }

    return 0;

}
