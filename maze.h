#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>

const int MAZE_SIZE = 25;
const int MINIMAP_SIZE = 200;
const int WINDOW_SIZE = 600;
const float PI = 3.14159265359f;

enum class MazeCell {
    Empty = 0,
    Path = 1,
    Goal = 2
};

class Maze {
private:
    int mazeData[2 * MAZE_SIZE - 1][2 * MAZE_SIZE - 1];
    sf::RenderTexture* mazeTexture;
    sf::Sprite* mazeSprite;
    
    void generateMaze(int arr[2 * MAZE_SIZE - 1][2 * MAZE_SIZE - 1]);
    void renderToTexture();

public:
    Maze();
    ~Maze();
    
    void updateMaze();
    void draw(sf::RenderWindow* window) const;
    MazeCell getCell(int row, int col) const;
    void printMaze() const;
    int getMazeSize() const { return 2 * MAZE_SIZE - 1; }
};

#endif // MAZE_H