#ifndef MAZE_H
#define MAZE_H

#include "SFML/Graphics.hpp"

#define NUMBER 25 // optimal size of maze
#define SW 200
#define BW 600
#define PI 3.1415

using namespace sf;

/**
 * Represents the maze for the game.
 * Stores the maze layout, handles rendering to a texture,
 * and provides methods to update and query cells.
 */
class Maze {
    int arr[2 * NUMBER - 1][2 * NUMBER - 1];
    RenderTexture* mazeTexture;
    Sprite* mazeSprite;

public:
    Maze();
    ~Maze();

    void draw(RenderWindow* window) const;
    void updateMaze();
    void renderMaze();
    void showMat() const;

    int getCell(int i, int j) const;
};

#endif // MAZE_H
