#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "maze.h"
#include "player.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Game {
private:
    GameConfig config;
    std::unique_ptr<Maze> maze;
    std::unique_ptr<Player> player;
    int score;
    float elapsedTime;
    sf::Font font;
    sf::Clock gameClock;

    void handleEvents(sf::RenderWindow& window);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void checkGoalCollection();
    void renderUI(sf::RenderWindow& window);
    void resetGame();

public:
    explicit Game(const GameConfig& cfg);
    void run();

    int getScore() const { return score; }
    float getElapsedTime() const { return elapsedTime; }
};

#endif // GAME_H