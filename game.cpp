#include "game.h"
#include <iostream>

Game::Game() : score(0), elapsedTime(0.0f) {
    const char* fontPaths[] = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:\\Windows\\Fonts\\arial.ttf"
    };
    
    for (const char* path : fontPaths) {
        if (font.openFromFile(path)) {
            break;
        }
    }
}

void Game::run() {
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(WINDOW_SIZE), static_cast<unsigned int>(WINDOW_SIZE)}), 
        "Raycasting Maze Game",
        sf::Style::Titlebar | sf::Style::Close
    );
    window.setFramerateLimit(60);
    
    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        
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
        
        if (event->is<sf::Event::KeyPressed>()) {
            const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
            if (keyEvent && keyEvent->code == sf::Keyboard::Key::U) {
                maze.updateMaze();
                player.reset();
                gameClock.restart();
                elapsedTime = 0.0f;
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (score < 3) {
        elapsedTime += deltaTime;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        player.move(maze);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        player.rotateLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        player.rotateRight();
    }
    
    checkGoalCollection();
    
    if (score >= 3) {
        resetGame();
    }
}

void Game::checkGoalCollection() {
    sf::Vector2f playerPos = player.getPosition();
    int cellX = static_cast<int>(playerPos.x);
    int cellY = static_cast<int>(playerPos.y);
    
    if (maze.getCell(cellY, cellX) == MazeCell::Goal) {
        score++;
        maze.updateMaze();
        player.reset();
        gameClock.restart();
        elapsedTime = 0.0f;
    }
}

void Game::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    player.render(&window, &maze);
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
    timerText.setPosition({static_cast<float>(WINDOW_SIZE - 60), 5.0f});
    window.draw(timerText);
}

void Game::resetGame() {
    score = 0;
    maze.updateMaze();
    player.reset();
    gameClock.restart();
    elapsedTime = 0.0f;
}