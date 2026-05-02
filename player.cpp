#include "player.h"
#include <iostream>
#include <cmath>
#include <algorithm>

pthread_mutex_t Player::renderMutex = PTHREAD_MUTEX_INITIALIZER;

Player::Player() 
    : position(INITIAL_X, INITIAL_Y),
      rotationAngle(0.0f),
      playerShape(PLAYER_RADIUS, 10) {
    
    playerShape.setPosition(position);
    playerShape.setOrigin(sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS));
    playerShape.setFillColor(sf::Color::Magenta);
    
    rays.reserve(RAY_COUNT);
    rayAngles.reserve(RAY_COUNT);
    
    for (int i = 0; i < RAY_COUNT; i++) {
        rays.push_back(Ray());
        float normalizedIndex = static_cast<float>(i) / RAY_COUNT;
        float angleOffset = std::atan(2.0f * (normalizedIndex - 0.5f));
        rayAngles.push_back(angleOffset);
    }
    
    wallStrips.resize(RAY_COUNT);
    
    int raysPerThread = RAY_COUNT / THREAD_COUNT;
    int remainder = RAY_COUNT % THREAD_COUNT;
    int startIndex = 0;
    
    for (int t = 0; t < THREAD_COUNT; t++) {
        int endIndex = startIndex + raysPerThread + (t < remainder ? 1 : 0);
        threadData.push_back(RaycastThreadData(startIndex, endIndex, nullptr, this, &wallStrips, nullptr));
        startIndex = endIndex;
    }
}

void Player::move(const Maze& maze) {
    if (wouldCollide(maze)) {
        return;
    }
    
    sf::Vector2f movement(
        PLAYER_SPEED * std::cos(rotationAngle),
        PLAYER_SPEED * std::sin(rotationAngle)
    );
    
    position += movement;
    playerShape.setPosition(position);
}

void Player::rotateLeft() {
    rotationAngle -= PLAYER_SPEED * 2.0f;
    while (rotationAngle < 0.0f) {
        rotationAngle += 2.0f * PI;
    }
}

void Player::rotateRight() {
    rotationAngle += PLAYER_SPEED * 2.0f;
    while (rotationAngle >= 2.0f * PI) {
        rotationAngle -= 2.0f * PI;
    }
}

void Player::reset() {
    position = sf::Vector2f(INITIAL_X, INITIAL_Y);
    rotationAngle = 0.0f;
    playerShape.setPosition(position);
}

bool Player::wouldCollide(const Maze& maze) const {
    sf::Vector2f nextPosition = position + sf::Vector2f(
        PLAYER_SPEED * std::cos(rotationAngle),
        PLAYER_SPEED * std::sin(rotationAngle)
    );
    
    int cellX = static_cast<int>(nextPosition.x);
    int cellY = static_cast<int>(nextPosition.y);
    
    return maze.getCell(cellY, cellX) == MazeCell::Empty;
}

float Player::getRayDirection(int rayIndex) const {
    float direction = rayAngles[rayIndex];
    direction += rotationAngle;
    
    while (direction > 2.0f * PI) {
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

void* processRayThread(void* args) {
    RaycastThreadData* data = static_cast<RaycastThreadData*>(args);
    
    sf::Vector2f playerPos = data->player->getPosition();
    float playerRot = data->player->getRotation();
    
    for (int i = data->startRayIndex; i < data->endRayIndex; i++) {
        float direction = data->player->getRayDirection(i);
        float cosAngle = std::cos(direction - playerRot);
        
        data->player->castRay(i, direction, playerPos, data->maze);
        
        float rayDistance = data->player->getRay(i)->getLength();
        float wallHeight = (static_cast<float>(WINDOW_SIZE) / 2.0f) / (rayDistance * cosAngle);
        
        sf::RectangleShape wallStrip(
            sf::Vector2f(static_cast<float>(WINDOW_SIZE) / RAY_COUNT, wallHeight)
        );
        
        float brightness = 255.0f * (1.0f - rayDistance / RAY_MAX_DISTANCE);
        brightness = std::max(0.0f, std::min(255.0f, brightness));
        wallStrip.setFillColor(sf::Color(0, static_cast<std::uint8_t>(brightness), 0));
        
        wallStrip.setPosition(
            sf::Vector2f(
                i * static_cast<float>(WINDOW_SIZE) / RAY_COUNT,
                0.4f * (WINDOW_SIZE - wallHeight)
            )
        );
        
        (*data->wallStrips)[i] = wallStrip;
        
        pthread_mutex_lock(&Player::renderMutex);
        data->window->draw(wallStrip);
        pthread_mutex_unlock(&Player::renderMutex);
    }
    
    return nullptr;
}

void Player::render(sf::RenderWindow* window, const Maze* maze) {
    renderBackground(window);
    
    for (auto& data : threadData) {
        data.maze = maze;
        data.window = window;
    }
    
    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_create(&threadIds[t], nullptr, processRayThread, &threadData[t]);
    }
    
    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_join(threadIds[t], nullptr);
    }
    
    maze->draw(window);
    renderMinimapPlayer(window, maze);
}

void Player::renderBackground(sf::RenderWindow* window) const {
    sf::RectangleShape sky(sf::Vector2f(WINDOW_SIZE, WINDOW_SIZE));
    sky.setFillColor(sf::Color(0, 0, 50));
    window->draw(sky);
    
    sf::RectangleShape floor(sf::Vector2f(WINDOW_SIZE, WINDOW_SIZE / 2.5f));
    floor.setPosition(sf::Vector2f(0, WINDOW_SIZE / 2.5f));
    floor.setFillColor(sf::Color(0, 50, 0));
    window->draw(floor);
}

void Player::renderMinimapPlayer(sf::RenderWindow* window, const Maze* maze) const {
    float minimapCellSize = static_cast<float>(MINIMAP_SIZE) / (2 * MAZE_SIZE - 1);
    
    sf::CircleShape minimapPlayer(1.0f, 5);
    minimapPlayer.setOrigin(sf::Vector2f(1.0f, 1.0f));
    minimapPlayer.setFillColor(sf::Color::Magenta);
    minimapPlayer.setScale(sf::Vector2f(minimapCellSize, minimapCellSize));
    
    minimapPlayer.setPosition(sf::Vector2f(
        position.x * minimapCellSize,
        WINDOW_SIZE - MINIMAP_SIZE + position.y * minimapCellSize
    ));
    
    window->draw(minimapPlayer);
}