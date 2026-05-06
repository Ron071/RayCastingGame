#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <string>

// Default configuration values
struct GameConfig {
    // Maze settings
    int mazeSize = 25;           // Size of the maze (odd numbers work best)

    // Raycasting settings
    int rayCount = 600;          // Number of rays to cast
    float rayMaxDistance = 8.0f; // Maximum ray casting distance

    // Threading settings
    int threadCount = 7;         // Number of threads for raycasting

    // Game settings
    int goalsToWin = 3;          // Number of goals to collect to win
    float playerSpeed = 0.05f;   // Player movement speed
    float rotationSpeed = 0.1f;  // Player rotation speed

    // Display settings
    int windowSize = 600;        // Window dimensions (square)
    int minimapSize = 200;       // Minimap dimensions (square)

    // Calculate derived values
    int getMazeGridSize() const { return 2 * mazeSize - 1; }
};

// Show configuration menu and return the chosen settings
GameConfig showConfigMenu();

#endif // CONFIG_H