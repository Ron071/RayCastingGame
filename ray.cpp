#include "ray.h"
#include <cmath>
#include <algorithm>

void Ray::cast(float angle, sf::Vector2f startPoint, const Maze* maze) {
    origin = startPoint;

    const sf::Vector2f direction(std::cos(angle), std::sin(angle));

    const double invSin = (std::sin(angle) != 0.0) ? 1.0 / std::sin(angle) : 1e30;
    const double invCos = (std::cos(angle) != 0.0) ? 1.0 / std::cos(angle) : 1e30;

    sf::Vector2f currentPosition = startPoint;
    float currentDistance = 0.0f;

    while (currentDistance < maxDistance) {
        const int cellX = static_cast<int>(currentPosition.x);
        const int cellY = static_cast<int>(currentPosition.y);

        const float dx = (direction.x > 0)
            ? (cellX + 1.0f - currentPosition.x)
            : (currentPosition.x - cellX);

        const float dy = (direction.y > 0)
            ? (cellY + 1.0f - currentPosition.y)
            : (currentPosition.y - cellY);

        const float stepX = std::fabs(dy * invSin);
        const float stepY = std::fabs(dx * invCos);
        const float step = std::min(stepX, stepY) + 1e-4f;

        currentDistance += step;
        currentPosition += direction * step;

        if (maze->getCell(static_cast<int>(currentPosition.y),
                          static_cast<int>(currentPosition.x)) == MazeCell::Empty) {
            break;
        }
    }

    if (currentDistance > maxDistance) {
        currentDistance = maxDistance;
    }

    hitPoint = startPoint + direction * currentDistance;
    rayLength = currentDistance;
}