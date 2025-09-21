#ifndef GAME_H
#define GAME_H

#include "maze.h"
#include "player.h"

/**
 * Main game class managing player, maze, score, and game loop.
 * Handles initialization, updating the game state, and rendering.
 */
class Game {
    Maze maze;       
    Player player;  
    int points;     

public:
    Game();
    void game();     
    void update();  
};

#endif // GAME_H
