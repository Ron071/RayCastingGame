#include "maze.h"
#include <random>
#include <iostream>
#include <chrono>

void Maze::generateMaze(int arr[2 * MAZE_SIZE - 1][2 * MAZE_SIZE - 1]) {
    int pathDecisions[MAZE_SIZE - 1][MAZE_SIZE - 1];
    
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
        seeded = true;
    }
    
    for (int i = 0; i < MAZE_SIZE - 1; i++) {
        for (int j = 0; j < MAZE_SIZE - 1; j++) {
            if (i == MAZE_SIZE - 2) {
                pathDecisions[i][j] = static_cast<int>(MazeCell::Path);
            } else if (j == MAZE_SIZE - 2) {
                pathDecisions[i][j] = static_cast<int>(MazeCell::Path);
            } else {
                pathDecisions[i][j] = (rand() % 2) ? static_cast<int>(MazeCell::Path) : static_cast<int>(MazeCell::Goal);
            }
        }
    }
    
    for (int i = 0; i < 2 * MAZE_SIZE - 1; i++) {
        for (int j = 0; j < 2 * MAZE_SIZE - 1; j++) {
            if (i == 0 || j == 0 || i == 2 * MAZE_SIZE - 2 || j == 2 * MAZE_SIZE - 2) {
                arr[i][j] = static_cast<int>(MazeCell::Empty);
            } else if (i % 2 == 1 && j % 2 == 1) {
                arr[i][j] = static_cast<int>(MazeCell::Path);
            } else {
                arr[i][j] = static_cast<int>(MazeCell::Empty);
            }
        }
    }
    
    for (int i = 0; i < MAZE_SIZE - 1; i++) {
        for (int j = 0; j < MAZE_SIZE - 1; j++) {
            if (i == MAZE_SIZE - 2 && j == MAZE_SIZE - 2) continue;
            
            if (pathDecisions[i][j] == static_cast<int>(MazeCell::Path)) {
                arr[2 * i + 2][2 * j + 1] = static_cast<int>(MazeCell::Path);
            } else {
                arr[2 * i + 1][2 * j + 2] = static_cast<int>(MazeCell::Path);
            }
        }
    }
    
    while (true) {
        int randomRow = rand() % (2 * MAZE_SIZE - 1);
        int randomCol = rand() % (2 * MAZE_SIZE - 1);
        
        if (arr[randomRow][randomCol] == static_cast<int>(MazeCell::Path)) {
            arr[randomRow][randomCol] = static_cast<int>(MazeCell::Goal);
            break;
        }
    }
}

Maze::Maze() {
    mazeTexture = new sf::RenderTexture({MINIMAP_SIZE, MINIMAP_SIZE});
    mazeSprite = new sf::Sprite(mazeTexture->getTexture());
    mazeSprite->setPosition(sf::Vector2f(0.0f, WINDOW_SIZE - MINIMAP_SIZE));
    
    updateMaze();
}

Maze::~Maze() {
    delete mazeTexture;
    delete mazeSprite;
}

void Maze::updateMaze() {
    generateMaze(mazeData);
    renderToTexture();
    
    sf::Image mazeImage = mazeTexture->getTexture().copyToImage();
    if (!mazeImage.saveToFile("maze.png")) {
        std::cerr << "Warning: Failed to save maze image" << std::endl;
    }
}

void Maze::draw(sf::RenderWindow* window) const {
    if (window && mazeSprite) {
        window->draw(*mazeSprite);
    }
}

MazeCell Maze::getCell(int row, int col) const {
    if (row < 0 || row >= 2 * MAZE_SIZE - 1 || col < 0 || col >= 2 * MAZE_SIZE - 1) {
        return MazeCell::Empty;
    }
    return static_cast<MazeCell>(mazeData[row][col]);
}

void Maze::printMaze() const {
    std::cout << "\n=== Maze Layout ===" << std::endl;
    for (int i = 0; i < 2 * MAZE_SIZE - 1; i++) {
        for (int j = 0; j < 2 * MAZE_SIZE - 1; j++) {
            std::cout << mazeData[i][j] << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << "===================\n" << std::endl;
}

void Maze::renderToTexture() {
    float cellSize = static_cast<float>(MINIMAP_SIZE) / (2 * MAZE_SIZE - 1);
    mazeTexture->clear(sf::Color::Black);
    
    sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
    
    for (int row = 0; row < 2 * MAZE_SIZE - 1; row++) {
        for (int col = 0; col < 2 * MAZE_SIZE - 1; col++) {
            MazeCell cellType = static_cast<MazeCell>(mazeData[row][col]);
            
            if (cellType == MazeCell::Empty) {
                cellShape.setFillColor(sf::Color::White);
                cellShape.setPosition(sf::Vector2f(col * cellSize, row * cellSize));
                mazeTexture->draw(cellShape);
            } else if (cellType == MazeCell::Goal) {
                cellShape.setFillColor(sf::Color::Red);
                cellShape.setPosition(sf::Vector2f(col * cellSize, row * cellSize));
                mazeTexture->draw(cellShape);
            }
        }
    }
    
    mazeTexture->display();
}