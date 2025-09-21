#include <iostream>
#include <cmath>
#include "maze.h"
#include "ray.h"
#include "player.h"

using namespace std;

/**
 * Returns the length of the ray
 */
float Ray::len() const {
    return length;
}

/**
 * Casts a ray from a starting position in a given direction.
 * Moves step-by-step until it hits a wall or reaches max length.
 */
void Ray::renderRay(float angle, Vector2f startPoint, const Maze* maze) {
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);
    double invSin = (sinAngle != 0.0) ? 1.0 / sinAngle : 1e30;
    double invCos = (cosAngle != 0.0) ? 1.0 / cosAngle : 1e30;

    Vector2f direction(cosAngle, sinAngle);
    Vector2f current = startPoint;
    float currentLength = 0.0f;
    const float EPS = 1e-4f;

    while (currentLength < RAY_LEN) {
        int cellX = static_cast<int>(current.x);
        int cellY = static_cast<int>(current.y);

        float dx = (cosAngle > 0) ? (cellX + 1.0f - current.x) : (current.x - cellX);
        float dy = (sinAngle > 0) ? (cellY + 1.0f - current.y) : (current.y - cellY);

        float stepX = fabs(dy * invSin);
        float stepY = fabs(dx * invCos);
        float step = min(stepX, stepY) + EPS;

        currentLength += step;
        current += direction * step;

        if (!maze->getCell(static_cast<int>(current.y), static_cast<int>(current.x))) {
            break;
        }
    }

    if (currentLength > RAY_LEN) {
        currentLength = RAY_LEN;
    }

    end = startPoint + direction * currentLength;
    length = currentLength;
}
