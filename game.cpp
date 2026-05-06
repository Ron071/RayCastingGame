#include "game.h"
#include <iostream>

namespace {
    const char* const FONT_PATHS[] = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:\\Windows\\Fonts\\arial.ttf"
    };
}

Game::Game(const GameConfig& cfg)
    : config(cfg),
      score(0),
      elapsedTime(0.0f) {

    // Calculate initial player position based on maze size
    const float initX = 1.5f;
    const float initY = 1.5f;

    maze = std::make_unique<Maze>(config.mazeSize, config.minimapSize);
    player = std::make_unique<Player>(
        config.rayCount,
        config.threadCount,
        config.playerSpeed,
        config.rotationSpeed,
        config.rayMaxDistance,
        0.1f,
        initX,
        initY);

    for (const char* path : FONT_PATHS) {
        if (font.openFromFile(path)) {
            break;
        }
    }
}

void Game::run() {
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(config.windowSize),
                       static_cast<unsigned int>(config.windowSize)}),
        "Raycasting Maze Game",
        sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        const float deltaTime = gameClock.restart().asSeconds();

        handleEvents(window);
        update(deltaTime);
        render(window);
    }
}

void Game::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }

        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->code == sf::Keyboard::Key::U) {
                maze->updateMaze();
                player->reset();
                gameClock.restart();
                elapsedTime = 0.0f;
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (score < config.goalsToWin) {
        elapsedTime += deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        player->move(*maze);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        player->rotateLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        player->rotateRight();
    }

    checkGoalCollection();

    if (score >= config.goalsToWin) {
        resetGame();
    }
}

void Game::checkGoalCollection() {
    const sf::Vector2f playerPos = player->getPosition();
    const int cellX = static_cast<int>(playerPos.x);
    const int cellY = static_cast<int>(playerPos.y);

    if (maze->getCell(cellY, cellX) == MazeCell::Goal) {
        ++score;
        maze->updateMaze();
        player->reset();
        gameClock.restart();
        elapsedTime = 0.0f;
    }
}

void Game::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    player->render(&window, maze.get());
    renderUI(window);
    window.display();
}

void Game::renderUI(sf::RenderWindow& window) {
    sf::Text scoreText(font, std::to_string(score), 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition({5.0f, 5.0f});
    window.draw(scoreText);

    sf::Text timerText(font, std::to_string(static_cast<int>(elapsedTime)), 24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition({static_cast<float>(config.windowSize - 60), 5.0f});
    window.draw(timerText);
}

void Game::resetGame() {
    score = 0;
    maze->updateMaze();
    player->reset();
    gameClock.restart();
    elapsedTime = 0.0f;
}