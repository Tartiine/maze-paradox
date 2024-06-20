#include "Game.h"
#include "Platform.h"
#include "Ground.h"
#include <iostream>
#include "TileMapManager.h"
#include "NoiseBasedGenerator.h"
#include "RuleBasedGenerator.h"
#include "TileMap.h"
#include "Animation.h"
#include <sstream>

using namespace std;

Game::Game() : showGamepadFlag(true) {
    this->initWindow();
    this->initRenderTexture();
    this->initMap();
    this->initPlayer();
    checkGamepad();
    this->initStartScreen();

    //TEST IA
    //this->trainModel();
    //this->testModel();
}

Game::~Game() {
    this->obstacles.clear();
}

void Game::collisionPlayer() {
    this->player->checkWindowBorders(this->renderTexture);

    auto renderedTileMaps = tileMapManager->getRenderedTileMaps();

    for (auto& tileMap : renderedTileMaps) {
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
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            if (!gameStarted) {
                gameStarted = true;
            }
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
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

    if (gameStarted) {
        this->updatePlayer(deltaTime);
        this->collisionPlayer();
        this->tileMapManager->update(deltaTime, this->player.get(), this->renderTexture);
        tileMapManager->updateAnimation(deltaTime);
        if (tileMapManager->checkPortal(this->player.get())) {
            showEndMenu();
        }
    }
}

void Game::renderPlayer(){
    this->player->render(this->renderTexture);
}

void Game::renderObstacles(bool debug) {
    tileMapManager->render(this->renderTexture, debug);
}

void Game::render() {
    if (!gameStarted) {
        this->initStartScreen();
        this->window.clear();
        this->window.draw(startBackground);
        this->window.draw(startText);
        this->window.draw(secondaryText);
        this->window.display();
        return;
    }

    this->renderTexture.clear(sf::Color(0, 120, 226));

    this->renderObstacles(false);
    this->renderPlayer();

    tileMapManager->render(this->renderTexture);
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

void Game::trainModel() {
    this->tileMapModel = make_unique<TileMapModel>(40*22, 1);
    this->tileMapModel->createModel("rb");
    this->tileMapModel->train("resources/rb_maps");
    this->tileMapModel->saveModel("resources/trained_model_rb.net");

    this->tileMapModel = make_unique<TileMapModel>(40*22, 1);
    this->tileMapModel->createModel("nb");
    this->tileMapModel->train("resources/nb_maps");
    this->tileMapModel->saveModel("resources/trained_model_nb.net");
}


void Game::createTriangle(bool gamepadConnected) { //TODO: Modify with message
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
    this->window.create(sf::VideoMode(this->resolution.x * this->scale, this->resolution.y * this->scale), "Maze Paradox", sf::Style::Close | sf::Style::Titlebar);
    this->window.setFramerateLimit(240);
    this->isFullscreenOn = false;
    this->fullscreenHorizontalOffset = 0;
    this->fullscreenVerticalOffset = 0;
}

void Game::initWindowFullscreen() {
    sf::VideoMode fullScreen = sf::VideoMode::getFullscreenModes()[0];

    this->window.create(fullScreen, "SFML Platformer", sf::Style::Fullscreen);
    this->window.setFramerateLimit(240);
    this->isFullscreenOn = true;
    this->scale = min(fullScreen.width / this->resolution.x, fullScreen.height / this->resolution.y);
    this->fullscreenHorizontalOffset = (fullScreen.width - (this->resolution.x * this->scale)) / 2;
    this->fullscreenVerticalOffset = (fullScreen.height - (this->resolution.y * this->scale)) / 2;
}

void Game::initRenderTexture() {
    this->renderTexture.create(this->resolution.x, this->resolution.y);
}

void Game::initPlayer() {
    this->player = make_unique<Player>(64, 300);
}

void Game::initMap() {
    auto rbGenerator = make_unique<RuleBasedGenerator>();
    auto nbGenerator = make_unique<NoiseBasedGenerator>();

    nbGenerator->generateBatch(25, 40, 22, "resources/maps/generated_map");
    rbGenerator->generateBatch(25, 40, 22, "resources/maps1/generated_map");

    
    this->tileMapModel = make_unique<TileMapModel>(40 * 22, 1);
    this->tileMapModel->testModel("resources/maps", "resources/trained_model_nb.net");
    this->tileMapModel->testModel("resources/maps1", "resources/trained_model_rb.net");
    
    
    tileMapManager = make_unique<TileMapManager>();
    vector<string> directories = {"resources/maps", "resources/maps1"};
    tileMapManager->generateTileMapOrder(directories, "resources/tile_map_order.txt", this->resolution.x, this->resolution.y);
    tileMapManager->loadTileMaps("resources/tile_map_order.txt");
    tileMapManager->createFinalMap();

}

void Game::initStartScreen() {

    if (!startFont.loadFromFile("resources/fonts/SuperSkinnyPixelBricks-3xKL.ttf")) {
        cerr << "Error: Could not load font 'resources/fonts/SuperSkinnyPixelBricks-3xKL.ttf'" << endl;
        this->window.close();
        return;
    }

    startText.setFont(startFont);
    startText.setString("MAZE PARADOX");
    startText.setCharacterSize(250);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = startText.getLocalBounds();
    startText.setOrigin(textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f);
    startText.setPosition(this->window.getSize().x / 2.0f - 20, this->window.getSize().y / 2.0f);
    if (!secondaryFont.loadFromFile("resources/fonts/computer-says-no.ttf")) {
        cerr << "Error: Could not load font 'resources/fonts/computer-says-no.ttf'" << endl;
        this->window.close();
        return;
    }

    secondaryText.setFont(secondaryFont);
    secondaryText.setString("Press Enter to Start");
    secondaryText.setCharacterSize(50);
    secondaryText.setFillColor(sf::Color::White);
    secondaryText.setStyle(sf::Text::Regular);

    sf::FloatRect secondaryTextRect = secondaryText.getLocalBounds();
    secondaryText.setOrigin(secondaryTextRect.left + secondaryTextRect.width / 2.0f,
                            secondaryTextRect.top + secondaryTextRect.height / 2.0f);
    secondaryText.setPosition(this->window.getSize().x / 2.0f,
                              startText.getPosition().y + textRect.height / 2.0f + secondaryTextRect.height / 2.0f + 20.f);

    startBackground.setSize(sf::Vector2f(this->window.getSize().x, this->window.getSize().y)); 
    startBackground.setFillColor(sf::Color::Black);
}

void Game::showEndMenu() {
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/computer-says-no.ttf")) {
        cerr << "Error: Could not load font 'resources/fonts/computer-says-no.ttf'" << endl;
        this->window.close();
        return;
    }

    sf::Text quitText("Press Q to Quit", font, 50);
    sf::Text replayText("Press R to Replay", font, 50);

    quitText.setFillColor(sf::Color::White);
    replayText.setFillColor(sf::Color::White);

    sf::FloatRect quitTextRect = quitText.getLocalBounds();
    quitText.setOrigin(quitTextRect.left + quitTextRect.width / 2.0f, quitTextRect.top + quitTextRect.height / 2.0f);
    quitText.setPosition(this->window.getSize().x / 2.0f, this->window.getSize().y / 2.0f - 50);

    sf::FloatRect replayTextRect = replayText.getLocalBounds();
    replayText.setOrigin(replayTextRect.left + replayTextRect.width / 2.0f, replayTextRect.top + replayTextRect.height / 2.0f);
    replayText.setPosition(this->window.getSize().x / 2.0f, this->window.getSize().y / 2.0f + 50);

    this->window.clear();
    this->window.draw(quitText);
    this->window.draw(replayText);
    this->window.display();

    bool choiceMade = false;
    while (!choiceMade) {
        sf::Event event;
        while (this->window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    this->window.close();
                    choiceMade = true;
                } else if (event.key.code == sf::Keyboard::R) {
                    this->resetGame(); 
                    choiceMade = true;
                }
            }
        }
    }
}

void Game::resetGame() {
    this->initPlayer();
    this->initMap();
    gameStarted = false;
}

//FIXME: fix trembling effects on border maps