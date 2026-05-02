#ifndef GAME_H
#define GAME_H

#include "maze.h"
#include "player.h"
#include <SFML/Graphics.hpp>

class Game {
private:
    Maze maze;
    Player player;
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
    Game();
    void run();
    int getScore() const { return score; }
    float getElapsedTime() const { return elapsedTime; }
};

#endif // GAME_H