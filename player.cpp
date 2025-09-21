#include "player.h"
#include <iostream>
#include <unistd.h>

/**
 * Constructor for Player.
 * Initializes player shape, rays, ray directions, shapes for rendering,
 * and prepares multithreaded raycasting data structures.
 */
Player::Player() : player(CircleShape(0.1f, 10)) {
    player.setPosition(Vector2f(1.5f, 1.5f));
    player.setOrigin(Vector2f(0.1f, 0.1f));

    for (int i = 0; i < RAYS; i++) {
        rays.push_back(Ray());
        rayDirections.push_back(atan(2 * (float)(i - RAYS / 2) / RAYS));
    }

    shapes.resize(RAYS);

    int raysPerThread = RAYS / THREAD_COUNT;
    int remainder = RAYS % THREAD_COUNT;
    int startIndex = 0;

    for (int t = 0; t < THREAD_COUNT; t++) {
        int endIndex = startIndex + raysPerThread + (t < remainder ? 1 : 0);
        threadData.push_back(Data(startIndex, endIndex, nullptr, this, &shapes, nullptr));
        startIndex = endIndex;
    }
}

/**
 * Returns the player's rotation in radians.
 */
float Player::rotation() {
    return player.getRotation().asRadians();
}

/**
 * Returns the player's current position.
 */
Vector2f Player::position() {
    return player.getPosition();
}

/**
 * Returns a pointer to the i-th ray.
 */
Ray* Player::ray(int i) {
    return &rays[i];
}

/**
 * Returns the direction of the i-th ray relative to the player.
 */
float Player::getRayDirection(int i) const {
    return rayDirections[i];
}

/**
 * Casts a ray at a given index from the player in a specified angle.
 * Updates the ray's length based on maze intersections.
 */
void Player::renderRay(int i, float angle, Vector2f startPoint, const Maze* maze) {
    rays[i].renderRay(angle, startPoint, maze);
}

// -------------------- Multithreaded raycasting --------------------
pthread_mutex_t windowMutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * Thread function to process a subset of rays.
 * Calculates ray intersection lengths, prepares shapes, and draws them safely.
 */
void* processRays(void* args) {
    Data* data = static_cast<Data*>(args);
    Vector2f current = data->player->position();
    float rotation = data->player->rotation();

    for (int i = data->startIndex; i < data->endIndex; ++i) {
        float direction = data->player->getRayDirection(i);
        float cosAngle = cos(direction);

        direction += rotation;
        if (direction > 2 * PI) direction -= 2 * PI;
        else if (direction < 0) direction += 2 * PI;

        data->player->renderRay(i, direction, current, data->maze);

        float height = ((float)BW / 2) / (data->player->ray(i)->len() * cosAngle);

        RectangleShape shape(Vector2f((float)BW / RAYS, height));
        shape.setFillColor(Color(0, 255 * (1 - (data->player->ray(i)->len() / RAY_LEN)), 0));
        shape.setPosition(Vector2f(i * (float)BW / RAYS, 0.4f * (BW - height)));

        (*data->shapes)[i] = shape;

        // Thread-safe draw
        pthread_mutex_lock(&windowMutex);
        data->window->draw(shape);
        pthread_mutex_unlock(&windowMutex);
    }

    return nullptr;
}

/**
 * Draws the player and all rays to the window.
 * Uses multithreading to calculate ray positions and shapes.
 */
void Player::draw(RenderWindow* window, const Maze* maze) {
    drawInterior(window);

    for (auto& d : threadData) {
        d.maze = maze;
        d.window = window;
    }

    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_create(&threads[t], nullptr, processRays, &threadData[t]);
    }
    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_join(threads[t], nullptr);
    }

    maze->draw(window);

    float cubeSize = (float)SW / (2 * NUMBER - 1);
    CircleShape player_on_screen(1, 5);
    player_on_screen.setOrigin({1, 1});
    player_on_screen.setFillColor(Color::Magenta);
    player_on_screen.setScale(Vector2f(cubeSize, cubeSize));
    player_on_screen.setPosition(Vector2f(position().x * cubeSize,
                                          BW - SW + position().y * cubeSize));
    window->draw(player_on_screen);
}

/**
 * Draws the static sky and floor.
 */
void Player::drawInterior(RenderWindow* window) const {
    RectangleShape sky(Vector2f(BW, BW));
    sky.setFillColor(Color(0, 0, 50));
    window->draw(sky);

    RectangleShape floor(Vector2f(BW, BW));
    floor.setPosition({0, BW / 2.5f});
    floor.setFillColor(Color(0, 50, 0));
    window->draw(floor);
}

/**
 * Checks if the next movement collides with maze walls.
 */
bool Player::checkCollision(const Maze& maze) {
    Vector2f nextPos = player.getPosition() +
                       Vector2f(SPEED * cos(rotation()), SPEED * sin(rotation()));
    return !(maze.getCell((int)nextPos.y, (int)nextPos.x));
}

/**
 * Moves the player forward if no collision.
 */
void Player::move(const Maze& maze) {
    if (!checkCollision(maze)) {
        player.move(Vector2f(SPEED * cos(rotation()), SPEED * sin(rotation())));
    }
}

/**
 * Rotates the player to the right.
 */
void Player::turnR() {
    player.rotate(degrees(100 * SPEED));
}

/**
 * Rotates the player to the left.
 */
void Player::turnL() {
    player.rotate(degrees(-100 * SPEED));
}

/**
 * Resets the player to the starting position and rotation.
 */
void Player::reset() {
    player.setPosition(Vector2f(1.5f, 1.5f));
    player.setRotation(radians(0));
}
