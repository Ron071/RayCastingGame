#include "player.h"
#include <cmath>
#include <algorithm>

pthread_mutex_t Player::renderMutex = PTHREAD_MUTEX_INITIALIZER;

void* processRayThread(void* args) {
    RaycastThreadData* data = static_cast<RaycastThreadData*>(args);

    const sf::Vector2f playerPos = data->player->getPosition();
    const float playerRot = data->player->getRotation();

    for (int i = data->startRayIndex; i < data->endRayIndex; ++i) {
        const float direction = data->player->getRayDirection(i);
        const float cosAngle = std::cos(direction - playerRot);

        data->player->castRay(i, direction, playerPos, data->maze);

        const float rayDistance = data->player->getRay(i)->getLength();
        const float wallHeight = (static_cast<float>(DEFAULT_WINDOW_SIZE) / 2.0f) /
                                  (rayDistance * cosAngle);

        float brightness = 255.0f * (1.0f - rayDistance / data->player->getRayMaxDistance());
        if (brightness < 0.0f) brightness = 0.0f;
        if (brightness > 255.0f) brightness = 255.0f;

        sf::RectangleShape wallStrip(
            sf::Vector2f(static_cast<float>(DEFAULT_WINDOW_SIZE) / data->player->getRayCount(), wallHeight));
        wallStrip.setFillColor(sf::Color(0, static_cast<std::uint8_t>(brightness), 0));
        wallStrip.setPosition(sf::Vector2f(
            i * static_cast<float>(DEFAULT_WINDOW_SIZE) / data->player->getRayCount(),
            0.4f * (DEFAULT_WINDOW_SIZE - wallHeight)));

        pthread_mutex_lock(&Player::renderMutex);
        (*data->wallStrips)[i] = wallStrip;
        data->window->draw(wallStrip);
        pthread_mutex_unlock(&Player::renderMutex);
    }

    return nullptr;
}

Player::Player(int rayCnt, int threadCnt, float speed, float rotSpeed,
               float maxDist, float radius, float initX, float initY)
    : playerShape(radius, 10),
      position(initX, initY),
      rotationAngle(0.0f),
      rayCount(rayCnt),
      threadCount(std::min(threadCnt, 16)),
      playerSpeed(speed),
      rotationSpeed(rotSpeed),
      rayMaxDistance(maxDist),
      playerRadius(radius),
      initialPosition(initX, initY) {

    playerShape.setPosition(position);
    playerShape.setOrigin(sf::Vector2f(radius, radius));
    playerShape.setFillColor(sf::Color::Magenta);

    rays.reserve(rayCount);
    rayAngles.reserve(rayCount);

    for (int i = 0; i < rayCount; ++i) {
        rays.emplace_back(position, rayMaxDistance);
        const float normalizedIndex = static_cast<float>(i) / rayCount;
        const float angleOffset = std::atan(2.0f * (normalizedIndex - 0.5f));
        rayAngles.push_back(angleOffset);
    }

    wallStrips.resize(rayCount);

    const int raysPerThread = rayCount / threadCount;
    const int remainder = rayCount % threadCount;
    int startIndex = 0;

    for (int t = 0; t < threadCount; ++t) {
        const int endIndex = startIndex + raysPerThread + (t < remainder ? 1 : 0);
        threadData.emplace_back(startIndex, endIndex);
        startIndex = endIndex;
    }
}

void Player::reset() {
    position = initialPosition;
    rotationAngle = 0.0f;
    playerShape.setPosition(position);
}

void Player::move(const Maze& maze) {
    if (wouldCollide(maze)) {
        return;
    }

    const sf::Vector2f movement(
        playerSpeed * std::cos(rotationAngle),
        playerSpeed * std::sin(rotationAngle));

    position += movement;
    playerShape.setPosition(position);
}

void Player::rotateLeft() {
    rotationAngle -= rotationSpeed;
    while (rotationAngle < 0.0f) {
        rotationAngle += 2.0f * PI;
    }
}

void Player::rotateRight() {
    rotationAngle += rotationSpeed;
    while (rotationAngle >= 2.0f * PI) {
        rotationAngle -= 2.0f * PI;
    }
}

bool Player::wouldCollide(const Maze& maze) const {
    const sf::Vector2f nextPosition(
        position.x + playerSpeed * std::cos(rotationAngle),
        position.y + playerSpeed * std::sin(rotationAngle));

    const int cellX = static_cast<int>(nextPosition.x);
    const int cellY = static_cast<int>(nextPosition.y);

    return maze.getCell(cellY, cellX) == MazeCell::Empty;
}

float Player::getRayDirection(int rayIndex) const {
    float direction = rayAngles[rayIndex] + rotationAngle;

    while (direction >= 2.0f * PI) {
        direction -= 2.0f * PI;
    }
    while (direction < 0.0f) {
        direction += 2.0f * PI;
    }

    return direction;
}

void Player::castRay(int rayIndex, float angle, sf::Vector2f startPoint, const Maze* maze) {
    rays[rayIndex].cast(angle, startPoint, maze);
}

void Player::render(sf::RenderWindow* window, const Maze* maze) {
    for (auto& data : threadData) {
        data.maze = maze;
        data.player = this;
        data.wallStrips = &wallStrips;
        data.window = window;
    }

    for (int t = 0; t < threadCount; ++t) {
        pthread_create(&threadIds[t], nullptr, processRayThread, &threadData[t]);
    }

    for (int t = 0; t < threadCount; ++t) {
        pthread_join(threadIds[t], nullptr);
    }

    maze->draw(window);
    renderMinimapPlayer(window, maze);
}

void Player::renderMinimapPlayer(sf::RenderWindow* window, const Maze* maze) const {
    const float minimapCellSize = static_cast<float>(maze->getMinimapSize()) / maze->getGridSize();

    sf::CircleShape minimapPlayer(0.5f, 5);
    minimapPlayer.setOrigin(sf::Vector2f(0.5f, 0.5f));
    minimapPlayer.setFillColor(sf::Color::Magenta);
    minimapPlayer.setScale(sf::Vector2f(minimapCellSize, minimapCellSize));
    minimapPlayer.setPosition(sf::Vector2f(
        position.x * minimapCellSize,
        DEFAULT_WINDOW_SIZE - maze->getMinimapSize() + position.y * minimapCellSize));

    window->draw(minimapPlayer);
}
