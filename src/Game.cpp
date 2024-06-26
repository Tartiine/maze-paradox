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
    loadFonts();
    initWindow();
    initRenderTexture();
    initMap();
    initPlayer();
    initStartScreen();
    checkGamepad();
}

Game::~Game() {
    obstacles.clear();
}

void Game::collisionPlayer() {
    player->checkWindowBorders(renderTexture);

    auto renderedTileMaps = tileMapManager->getRenderedTileMaps();

    for (auto& tileMap : renderedTileMaps) {
        for (unsigned i = 0; i < tileMap->getHeight(); ++i) {
            for (unsigned j = 0; j < tileMap->getWidth(); ++j) {
                auto& obstacle = tileMap->getTile(i, j);
                if (obstacle) {
                    sf::FloatRect bounds = obstacle->getHitbox();
                    if (player->isColliding(bounds)) {
                        player->resolveCollision(bounds);
                    }
                }
            }
        }
    }
}

void Game::updatePlayer(float deltaTime) {
    player->update(deltaTime);
}

void Game::update(float deltaTime) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            window.close();
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            if (!gameStarted) {
                gameStarted = true;
            }
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F) {
            window.close();
            if (isFullscreenOn) {
                scale = 2;
                window.setMouseCursorVisible(true);
                initWindow();
            } else {
                window.setMouseCursorVisible(false);
                initWindowFullscreen();  
            }
        } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) {
            scale = (scale % 3) + 1;
            window.close();
            initWindow();
        }
    }

    if (gameStarted) {
        updatePlayer(deltaTime);
        collisionPlayer();
        tileMapManager->update(deltaTime, player.get(), renderTexture);
        tileMapManager->updateAnimation(deltaTime);
        if (tileMapManager->checkPortal(player.get())) {
            showEndMenu();
        }
    }
}

void Game::renderPlayer(){
    player->render(renderTexture);
}

void Game::renderObstacles(bool debug) {
    tileMapManager->render(renderTexture, debug);
}

void Game::render() {
    if (!gameStarted) {
        initStartScreen();
        window.clear();
        window.draw(startBackground);
        window.draw(startText);
        window.draw(secondaryText);
        window.display();
        return;
    }

    renderTexture.clear(sf::Color(0, 120, 226));

    renderObstacles(false);
    renderPlayer();

    renderTexture.display();

    sf::Sprite renderSprite(renderTexture.getTexture());
    renderSprite.setScale(scale, scale);
    renderSprite.setPosition(fullscreenHorizontalOffset, fullscreenVerticalOffset);
    
    window.clear();
    window.draw(renderSprite);

    if (showGamepadFlag && infoClock.getElapsedTime().asSeconds() < 3) {
        window.draw(gamepadText);
    } else {
        showGamepadFlag = false;
    }

    window.display();  
}

const sf::RenderWindow& Game::getWindow() const {
    return window;
}

void Game::checkGamepad() {
    bool gamepadConnected = false;
    for (unsigned int i = 0; i < sf::Joystick::Count; ++i) {
        if (sf::Joystick::isConnected(i)) {
            gamepadConnected = true;
            break;
        }
    }
    createGamepadText(gamepadConnected);
}

void Game::trainModel() {
    tileMapModel = make_unique<TileMapModel>(40*22, 1);
    tileMapModel->createModel("rb");
    tileMapModel->train("resources/rb_maps");
    tileMapModel->saveModel("resources/trained_model_rb.net");

    tileMapModel = make_unique<TileMapModel>(40*22, 1);
    tileMapModel->createModel("nb");
    tileMapModel->train("resources/nb_maps");
    tileMapModel->saveModel("resources/trained_model_nb.net");
}


void Game::createGamepadText(bool gamepadConnected) {
    gamepadText.setFont(secondaryFont);
    if (gamepadConnected) {
        gamepadText.setString("Gamepad Connected");
        gamepadText.setFillColor(sf::Color::Green);
    } else {
        gamepadText.setString("Gamepad Not Connected");
        gamepadText.setFillColor(sf::Color::Red);
    }
    gamepadText.setCharacterSize(30);
    gamepadText.setPosition(10.f, 10.f);
}

void Game::initWindow() {
    window.create(sf::VideoMode(resolution.x * scale, resolution.y * scale), "Maze Paradox", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(240);
    isFullscreenOn = false;
    fullscreenHorizontalOffset = 0;
    fullscreenVerticalOffset = 0;
}

void Game::initWindowFullscreen() {
    sf::VideoMode fullScreen = sf::VideoMode::getFullscreenModes()[0];

    window.create(fullScreen, "SFML Platformer", sf::Style::Fullscreen);
    window.setFramerateLimit(240);
    isFullscreenOn = true;
    scale = min(fullScreen.width / resolution.x, fullScreen.height / resolution.y);
    fullscreenHorizontalOffset = (fullScreen.width - (resolution.x * scale)) / 2;
    fullscreenVerticalOffset = (fullScreen.height - (resolution.y * scale)) / 2;
}

void Game::initRenderTexture() {
    renderTexture.create(resolution.x, resolution.y);
}

void Game::initPlayer() {
    player = make_unique<Player>(64, 300);
}

void Game::initMap() {
    /*
    auto rbGenerator = make_unique<RuleBasedGenerator>();
    auto nbGenerator = make_unique<NoiseBasedGenerator>();

    nbGenerator->generateBatch(25, 40, 22, "resources/maps/generated_map");
    rbGenerator->generateBatch(25, 40, 22, "resources/maps1/generated_map");

    
    tileMapModel = make_unique<TileMapModel>(40 * 22, 1);
    tileMapModel->testModel("resources/maps", "resources/trained_model_nb.net");
    tileMapModel->testModel("resources/maps1", "resources/trained_model_rb.net");
    */
    
    tileMapManager = make_unique<TileMapManager>();
    vector<string> directories = {"resources/maps", "resources/maps1"};
    tileMapManager->generateTileMapOrder(directories, "resources/tile_map_order.txt", resolution.x, resolution.y);
    tileMapManager->loadTileMaps("resources/tile_map_order.txt");
    tileMapManager->createFinalMap();

}

void Game::loadFonts(){
    if (!startFont.loadFromFile("resources/fonts/SuperSkinnyPixelBricks-3xKL.ttf")) {
        cerr << "Error: Could not load font 'resources/fonts/SuperSkinnyPixelBricks-3xKL.ttf'" << endl;
        window.close();
        return;
    }

    if (!secondaryFont.loadFromFile("resources/fonts/computer-says-no.ttf")) {
        cerr << "Error: Could not load font 'resources/fonts/computer-says-no.ttf'" << endl;
        window.close();
        return;
    }
}

void Game::initStartScreen()
{
    startText.setFont(startFont);
    startText.setString("MAZE PARADOX");
    startText.setCharacterSize(250);
    startText.setFillColor(sf::Color::White);
    startText.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = startText.getLocalBounds();
    startText.setOrigin(textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f);
    startText.setPosition(window.getSize().x / 2.0f - 20, window.getSize().y / 2.0f);

    secondaryText.setFont(secondaryFont);
    secondaryText.setString("Press Enter to Start");
    secondaryText.setCharacterSize(50);
    secondaryText.setFillColor(sf::Color::White);
    secondaryText.setStyle(sf::Text::Regular);

    sf::FloatRect secondaryTextRect = secondaryText.getLocalBounds();
    secondaryText.setOrigin(secondaryTextRect.left + secondaryTextRect.width / 2.0f,
                            secondaryTextRect.top + secondaryTextRect.height / 2.0f);
    secondaryText.setPosition(window.getSize().x / 2.0f,
                              startText.getPosition().y + textRect.height / 2.0f + secondaryTextRect.height / 2.0f + 20.f);

    startBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y)); 
    startBackground.setFillColor(sf::Color::Black);
}

void Game::showEndMenu() {
    sf::Text quitText("Press Q to Quit", secondaryFont, 50);
    sf::Text replayText("Press R to Replay", secondaryFont, 50);

    quitText.setFillColor(sf::Color::White);
    replayText.setFillColor(sf::Color::White);

    sf::FloatRect quitTextRect = quitText.getLocalBounds();
    quitText.setOrigin(quitTextRect.left + quitTextRect.width / 2.0f, quitTextRect.top + quitTextRect.height / 2.0f);
    quitText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 50);

    sf::FloatRect replayTextRect = replayText.getLocalBounds();
    replayText.setOrigin(replayTextRect.left + replayTextRect.width / 2.0f, replayTextRect.top + replayTextRect.height / 2.0f);
    replayText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 50);

    window.clear();
    window.draw(quitText);
    window.draw(replayText);
    window.display();

    bool choiceMade = false;
    while (!choiceMade) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Q) {
                    window.close();
                    choiceMade = true;
                } else if (event.key.code == sf::Keyboard::R) {
                    resetGame(); 
                    choiceMade = true;
                }
            }
        }
    }
}

void Game::resetGame() {
    initPlayer();
    initMap();
    gameStarted = false;
}

//TODO: Gamepad message when gamepad connected or disconnected
//FIXME: fix trembling effects on border maps