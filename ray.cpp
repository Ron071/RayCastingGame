#include "ray.h"
#include <cmath>
#include <algorithm>

void Ray::cast(float angle, sf::Vector2f startPoint, const Maze* maze) {
    origin = startPoint;
    
    sf::Vector2f direction(std::cos(angle), std::sin(angle));
    
    double invSin = (std::sin(angle) != 0.0) ? 1.0 / std::sin(angle) : 1e30;
    double invCos = (std::cos(angle) != 0.0) ? 1.0 / std::cos(angle) : 1e30;
    
    sf::Vector2f currentPosition = startPoint;
    float currentDistance = 0.0f;
    
    while (currentDistance < RAY_MAX_DISTANCE) {
        int cellX = static_cast<int>(currentPosition.x);
        int cellY = static_cast<int>(currentPosition.y);
        
        float dx, dy;
        if (direction.x > 0) {
            dx = (cellX + 1.0f - currentPosition.x);
        } else {
            dx = (currentPosition.x - cellX);
        }
        
        if (direction.y > 0) {
            dy = (cellY + 1.0f - currentPosition.y);
        } else {
            dy = (currentPosition.y - cellY);
        }
        
        float stepX = std::fabs(dy * invSin);
        float stepY = std::fabs(dx * invCos);
        
        float step = std::min(stepX, stepY) + RAY_EPSILON;
        
        currentDistance += step;
        currentPosition += direction * step;
        
        if (maze->getCell(static_cast<int>(currentPosition.y), 
                          static_cast<int>(currentPosition.x)) == MazeCell::Empty) {
            break;
        }
    }
    
    if (currentDistance > RAY_MAX_DISTANCE) {
        currentDistance = RAY_MAX_DISTANCE;
    }
    
    hitPoint = startPoint + direction * currentDistance;
    rayLength = currentDistance;
}