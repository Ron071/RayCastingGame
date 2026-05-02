#ifndef PLAYER_H
#define PLAYER_H

#include "ray.h"
#include "maze.h"
#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <vector>

const float PLAYER_SPEED = 0.05f;
const int RAY_COUNT = 600;
const float FIELD_OF_VIEW = 90.0f;
const int THREAD_COUNT = 7;
const float PLAYER_RADIUS = 0.1f;
const float INITIAL_X = 1.5f;
const float INITIAL_Y = 1.5f;

class Player;

struct RaycastThreadData {
    int startRayIndex;
    int endRayIndex;
    const Maze* maze;
    Player* player;
    std::vector<sf::RectangleShape>* wallStrips;
    sf::RenderWindow* window;
    
    RaycastThreadData(int start, int end, const Maze* m, Player* p,
                      std::vector<sf::RectangleShape>* strips, sf::RenderWindow* w)
        : startRayIndex(start), endRayIndex(end), maze(m),
          player(p), wallStrips(strips), window(w) {}
};

class Player {
private:
    sf::CircleShape playerShape;
    sf::Vector2f position;
    float rotationAngle;
    
    std::vector<Ray> rays;
    std::vector<float> rayAngles;
    std::vector<sf::RectangleShape> wallStrips;
    
    std::vector<RaycastThreadData> threadData;
    pthread_t threadIds[THREAD_COUNT];
    
    static pthread_mutex_t renderMutex;
    
    float getRayDirection(int rayIndex) const;
    bool wouldCollide(const Maze& maze) const;
    void renderBackground(sf::RenderWindow* window) const;
    void renderMinimapPlayer(sf::RenderWindow* window, const Maze* maze) const;

public:
    Player();
    
    sf::Vector2f getPosition() const { return position; }
    float getRotation() const { return rotationAngle; }
    Ray* getRay(int index) { return &rays[index]; }
    
    void reset();
    void move(const Maze& maze);
    void rotateLeft();
    void rotateRight();
    
    void render(sf::RenderWindow* window, const Maze* maze);
    void castRay(int rayIndex, float angle, sf::Vector2f startPoint, const Maze* maze);
    
    friend void* processRayThread(void* args);
};

#endif // PLAYER_H