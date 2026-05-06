#ifndef MAZE_H
#define MAZE_H

#include <SFML/Graphics.hpp>

// Maximum maze size (must be odd)
constexpr int MAX_MAZE_SIZE = 51;
constexpr int MAX_GRID_SIZE = 2 * MAX_MAZE_SIZE - 1;

// Default display constants
constexpr int DEFAULT_MINIMAP_SIZE = 200;
constexpr int DEFAULT_WINDOW_SIZE = 600;
constexpr float PI = 3.14159265359f;

// Cell types in the maze
enum class MazeCell {
    Empty = 0,  // Wall
    Path = 1,   // Walkable path
    Goal = 2    // Goal/target
};

class Maze {
private:
    int mazeData[MAX_GRID_SIZE][MAX_GRID_SIZE];
    sf::RenderTexture mazeTexture;
    sf::Sprite mazeSprite;
    int mazeSize;        // Actual maze size (odd number)
    int gridSize;        // = 2 * mazeSize - 1
    int minimapSize;     // Minimap display size

    void generateMaze();
    void renderToTexture();

public:
    Maze(int size = 25, int minimapSz = 200);

    void updateMaze();
    void draw(sf::RenderWindow* window) const;
    MazeCell getCell(int row, int col) const;
    void printMaze() const;
    int getMazeSize() const { return mazeSize; }
    int getGridSize() const { return gridSize; }
    int getMinimapSize() const { return minimapSize; }
};

#endif // MAZE_H