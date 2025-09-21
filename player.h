#ifndef PLAYER_H
#define PLAYER_H

#include "ray.h"
#include "maze.h"
#include "SFML/Graphics.hpp"
#include <pthread.h>
#include <vector>

using namespace sf;
using namespace std;

#define SPEED 0.05
#define RAYS 600
#define ANGLE 90
#define THREAD_COUNT 7

/**
 * Represents the player in the game.
 * Handles movement, rotation, raycasting, and rendering.
 * Uses multithreading to cast rays efficiently and stores the results for rendering.
 */
struct Data {
    int startIndex;
    int endIndex;
    const Maze* maze;
    class Player* player;
    vector<RectangleShape>* shapes;
    RenderWindow* window;

    Data(int start, int end, const Maze* maze, Player* player,
         vector<RectangleShape>* shapes, RenderWindow* window)
        : startIndex(start), endIndex(end), maze(maze),
          player(player), shapes(shapes), window(window) {}
};

class Player {
    CircleShape player;
    vector<Ray> rays;
    vector<float> rayDirections;
    vector<RectangleShape> shapes;
    vector<Data> threadData;
    pthread_t threads[THREAD_COUNT];

public:
    Player();

    void draw(RenderWindow* window, const Maze* maze);
    void drawInterior(RenderWindow* window) const;

    Vector2f position();
    float rotation();
    float getRayDirection(int i) const;
    Ray* ray(int i);
    void renderRay(int i, float angle, Vector2f startPoint, const Maze* maze);

    bool checkCollision(const Maze& maze);
    void move(const Maze& maze);
    void turnL();
    void turnR();
    void reset();
};

#endif // PLAYER_H
