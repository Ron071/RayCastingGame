#include "game.h"
#include <iostream>

/**
 * Constructor for Game.
 * Initializes the score to 0.
 */
Game::Game() : points(0) {}

/**
 * Main game loop.
 * Handles input, updates player and maze, manages scoring,
 * and renders everything including points and timer.
 */
void Game::game() {
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(Vector2u(BW, BW)), "GAME");
    window.setFramerateLimit(60);

    sf::Font font("/System/Library/Fonts/Supplemental/Arial.ttf");
    sf::Clock timerClock;
    float elapsedTime = 0.f;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                exit(0);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::W)) player.move(maze);
        if (Keyboard::isKeyPressed(Keyboard::Key::A)) player.turnL();
        if (Keyboard::isKeyPressed(Keyboard::Key::D)) player.turnR();
        if (Keyboard::isKeyPressed(Keyboard::Key::U)) {
            maze.updateMaze();
            player.reset();
            timerClock.restart();
        }

        if (maze.getCell((int)player.position().y, (int)player.position().x) == 2) {
            points++;
            maze.updateMaze();
            player.reset();
        }

        if (points < 3) elapsedTime = timerClock.getElapsedTime().asSeconds();

        window.clear();
        player.draw(&window, &maze);

        sf::Text pointsText(font);
        pointsText.setString(std::to_string(points));
        pointsText.setCharacterSize(24);
        pointsText.setFillColor(sf::Color::White);
        pointsText.setPosition({5, 5});
        window.draw(pointsText);

        // Draw timer (top-right)
        sf::Text timerText(font);
        timerText.setString(std::to_string(static_cast<int>(elapsedTime)));
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition({BW - 60, 5});
        window.draw(timerText);

        window.display();
    }
}

/**
 * Resets the game state.
 * Resets player position and regenerates the maze.
 */
void Game::update() {
    player.reset();
    maze.updateMaze();
}
