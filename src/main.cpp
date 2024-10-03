#include "Game.h"

int main() {
    Game game;
    sf::Clock clock;

    const float TICK_RATE = 1.f / 60.f;
    float elapsedTime = 0.f;

    while (game.getWindow().isOpen()) {
        elapsedTime += clock.restart().asSeconds();

        while (elapsedTime > TICK_RATE) {
            game.update(TICK_RATE);
            elapsedTime -= TICK_RATE;
        }
               
        game.render();
    }

    return 0;

}
