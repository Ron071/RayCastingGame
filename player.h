#ifndef PLAYER_H
#define PLAYER_H

#include "ray.h"
#include <SFML/Graphics.hpp>
#include <pthread.h>
#include <vector>

// Default constants (can be overridden by config)
constexpr float DEFAULT_PLAYER_SPEED = 0.05f;
constexpr int DEFAULT_RAY_COUNT = 600;
constexpr float FIELD_OF_VIEW = 90.0f;
constexpr int DEFAULT_THREAD_COUNT = 7;
constexpr float DEFAULT_PLAYER_RADIUS = 0.1f;
constexpr float DEFAULT_INITIAL_X = 1.5f;
constexpr float DEFAULT_INITIAL_Y = 1.5f;

class Player;

struct RaycastThreadData {
    int startRayIndex;
    int endRayIndex;
    const Maze* maze;
    Player* player;
    std::vector<sf::RectangleShape>* wallStrips;
    sf::RenderWindow* window;

    RaycastThreadData(int start, int end)
        : startRayIndex(start), endRayIndex(end), maze(nullptr),
          player(nullptr), wallStrips(nullptr), window(nullptr) {}
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
    pthread_t threadIds[16];  // Max threads

    static pthread_mutex_t renderMutex;

    // Configuration
    int rayCount;
    int threadCount;
    float playerSpeed;
    float rotationSpeed;
    float rayMaxDistance;
    float playerRadius;
    sf::Vector2f initialPosition;

    float getRayDirection(int rayIndex) const;
    bool wouldCollide(const Maze& maze) const;
    void renderMinimapPlayer(sf::RenderWindow* window, const Maze* maze) const;

    friend void* processRayThread(void* args);

public:
    Player(int rays = DEFAULT_RAY_COUNT,
           int threads = DEFAULT_THREAD_COUNT,
           float speed = DEFAULT_PLAYER_SPEED,
           float rotSpeed = 0.1f,
           float maxDist = 8.0f,
           float radius = DEFAULT_PLAYER_RADIUS,
           float initX = DEFAULT_INITIAL_X,
           float initY = DEFAULT_INITIAL_Y);

    sf::Vector2f getPosition() const { return position; }
    float getRotation() const { return rotationAngle; }
    Ray* getRay(int index) { return &rays[index]; }
    int getRayCount() const { return rayCount; }
    float getRayMaxDistance() const { return rayMaxDistance; }
    int getThreadCount() const { return threadCount; }
    float getPlayerSpeed() const { return playerSpeed; }
    float getRotationSpeed() const { return rotationSpeed; }

    void reset();
    void move(const Maze& maze);
    void rotateLeft();
    void rotateRight();
    void render(sf::RenderWindow* window, const Maze* maze);
    void castRay(int rayIndex, float angle, sf::Vector2f startPoint, const Maze* maze);
};

#endif // PLAYER_H