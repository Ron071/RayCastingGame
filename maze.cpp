#include "maze.h"
#include <chrono>
#include <iostream>

namespace {
    constexpr int CARVE_HORIZONTAL = 1;
    constexpr int CARVE_VERTICAL = 2;
}

void Maze::generateMaze() {
    int pathDecisions[MAX_MAZE_SIZE - 1][MAX_MAZE_SIZE - 1];

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(
            std::chrono::system_clock::now().time_since_epoch().count()));
        seeded = true;
    }

    for (int i = 0; i < mazeSize - 1; ++i) {
        for (int j = 0; j < mazeSize - 1; ++j) {
            if (i == mazeSize - 2) {
                pathDecisions[i][j] = CARVE_VERTICAL;
            } else if (j == mazeSize - 2) {
                pathDecisions[i][j] = CARVE_HORIZONTAL;
            } else {
                pathDecisions[i][j] = (std::rand() % 2) ? CARVE_HORIZONTAL : CARVE_VERTICAL;
            }
        }
    }

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            if (i == 0 || j == 0 || i == gridSize - 1 || j == gridSize - 1) {
                mazeData[i][j] = static_cast<int>(MazeCell::Empty);
            } else if (i % 2 == 1 && j % 2 == 1) {
                mazeData[i][j] = static_cast<int>(MazeCell::Path);
            } else {
                mazeData[i][j] = static_cast<int>(MazeCell::Empty);
            }
        }
    }

    for (int i = 0; i < mazeSize - 1; ++i) {
        for (int j = 0; j < mazeSize - 1; ++j) {
            if (i == mazeSize - 2 && j == mazeSize - 2) continue;
            if (pathDecisions[i][j] == CARVE_HORIZONTAL) {
                mazeData[2 * i + 2][2 * j + 1] = static_cast<int>(MazeCell::Path);
            } else {
                mazeData[2 * i + 1][2 * j + 2] = static_cast<int>(MazeCell::Path);
            }
        }
    }

    while (true) {
        int row = std::rand() % gridSize;
        int col = std::rand() % gridSize;
        if (mazeData[row][col] == static_cast<int>(MazeCell::Path)) {
            mazeData[row][col] = static_cast<int>(MazeCell::Goal);
            break;
        }
    }
}

void Maze::renderToTexture() {
    const float cellSize = static_cast<float>(minimapSize) / gridSize;

    mazeTexture.clear(sf::Color(50, 50, 50));

    sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));

    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            MazeCell cellType = static_cast<MazeCell>(mazeData[row][col]);

            if (cellType == MazeCell::Empty) {
                cellShape.setFillColor(sf::Color::White);
            } else if (cellType == MazeCell::Goal) {
                cellShape.setFillColor(sf::Color::Red);
            } else {
                continue;
            }

            cellShape.setPosition(sf::Vector2f(col * cellSize, row * cellSize));
            mazeTexture.draw(cellShape);
        }
    }

    mazeTexture.display();
}

Maze::Maze(int size, int minimapSz)
    : mazeSize(size),
      gridSize(2 * size - 1),
      minimapSize(minimapSz),
      mazeTexture(sf::Vector2u(minimapSz, minimapSz)),
      mazeSprite(mazeTexture.getTexture()) {
    mazeSprite.setPosition(sf::Vector2f(0.0f, DEFAULT_WINDOW_SIZE - minimapSize));
    updateMaze();
}

void Maze::updateMaze() {
    generateMaze();
    renderToTexture();

    sf::Image mazeImage = mazeTexture.getTexture().copyToImage();
    if (!mazeImage.saveToFile("maze.png")) {
        std::cerr << "Warning: Failed to save maze image\n";
    }
}

void Maze::draw(sf::RenderWindow* window) const {
    if (window) {
        window->draw(mazeSprite);
    }
}

MazeCell Maze::getCell(int row, int col) const {
    if (row < 0 || row >= gridSize || col < 0 || col >= gridSize) {
        return MazeCell::Empty;
    }
    return static_cast<MazeCell>(mazeData[row][col]);
}

void Maze::printMaze() const {
    std::cout << "\n=== Maze Layout ===\n";
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            std::cout << mazeData[i][j] << "  ";
        }
        std::cout << '\n';
    }
    std::cout << "===================\n\n";
}