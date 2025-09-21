#include "maze.h"
#include <random>
#include <iostream>

using namespace std;

/**
 * Generate a new maze layout.
 * Fills helper matrix, builds base grid, carves connections, and places a goal.
 */
static void update(int arr[2 * NUMBER - 1][2 * NUMBER - 1]) {
    int temp[NUMBER - 1][NUMBER - 1];

    for (int i = 0; i < NUMBER - 1; i++) {
        for (int j = 0; j < NUMBER - 1; j++) {
            if (i == NUMBER - 2) temp[i][j] = 2;
            else if (j == NUMBER - 2) temp[i][j] = 1;
            else temp[i][j] = (rand() % 2) ? 1 : 2;
        }
    }

    for (int i = 0; i < 2 * NUMBER - 1; i++) {
        for (int j = 0; j < 2 * NUMBER - 1; j++) {
            if (i == 0 || j == 0 || i == 2 * NUMBER - 2 || j == 2 * NUMBER - 2)
                arr[i][j] = 0;
            else if (i % 2 && j % 2) arr[i][j] = 1;
            else arr[i][j] = 0;
        }
    }

    for (int i = 0; i < NUMBER - 1; i++) {
        for (int j = 0; j < NUMBER - 1; j++) {
            if (i == NUMBER - 2 && j == NUMBER - 2) continue;
            if (temp[i][j] == 1) arr[2 * i + 2][2 * j + 1] = 1;
            else arr[2 * i + 1][2 * j + 2] = 1;
        }
    }

    while (true) {
        int i = rand() % (2 * NUMBER - 1);
        int j = rand() % (2 * NUMBER - 1);
        if (arr[i][j] == 1) {
            arr[i][j] = 2;
            return;
        }
    }
}

/**
 * Constructor for Maze.
 * Initializes texture and sprite, and generates initial maze.
 */
Maze::Maze() {
    mazeTexture = new RenderTexture({SW, SW});
    mazeSprite = new Sprite(mazeTexture->getTexture());
    mazeSprite->move(Vector2f(0, BW - SW));
    updateMaze();
}

/**
 * Destructor for Maze.
 * Cleans up allocated texture and sprite.
 */
Maze::~Maze() {
    delete mazeTexture;
    delete mazeSprite;
}

/**
 * Renders the maze into its texture.
 * Colors walls white, paths black, goal red.
 */
void Maze::renderMaze() {
    float cubeSize = (float)SW / (2 * NUMBER - 1);
    mazeTexture->clear(Color::Black);

    RectangleShape cube({cubeSize, cubeSize});
    for (int i = 0; i < 2 * NUMBER - 1; i++) {
        for (int j = 0; j < 2 * NUMBER - 1; j++) {
            if (arr[i][j] == 0) cube.setFillColor(Color::White);
            else if (arr[i][j] == 2) cube.setFillColor(Color::Red);
            else continue;

            cube.setPosition(Vector2f(cubeSize * j, cubeSize * i));
            mazeTexture->draw(cube);
        }
    }
    mazeTexture->display();
}

/**
 * Draws the maze sprite on a window.
 */
void Maze::draw(RenderWindow* window) const {
    window->draw(*mazeSprite);
}

/**
 * Returns the value of a cell at (i, j).
 */
int Maze::getCell(int i, int j) const {
    return arr[i][j];
}

/**
 * Updates the maze by generating a new layout and rendering it.
 * Also saves the maze as an image.
 */
void Maze::updateMaze() {
    update(arr);
    renderMaze();

    Image img = mazeTexture->getTexture().copyToImage();
    if (!img.saveToFile("maze.png")) {
        cout << "FAILED TO SAVE MAZE IMAGE" << endl;
    }
}

/**
 * Prints the maze layout to console.
 */
void Maze::showMat() const {
    for (int i = 0; i < 2 * NUMBER - 1; i++) {
        for (int j = 0; j < 2 * NUMBER - 1; j++) {
            cout << arr[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl << endl;
}
