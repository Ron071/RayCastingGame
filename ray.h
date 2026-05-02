#ifndef RAY_H
#define RAY_H

#include "maze.h"
#include <SFML/Graphics.hpp>

const float RAY_MAX_DISTANCE = 8.0f;
const float RAY_EPSILON = 1e-4f;

class Ray {
private:
    sf::Vector2f origin;
    sf::Vector2f hitPoint;
    float rayLength;

public:
    Ray() = default;
    explicit Ray(sf::Vector2f startOrigin) 
        : origin(startOrigin), hitPoint(startOrigin), rayLength(0.0f) {}
    
    float getLength() const { return rayLength; }
    sf::Vector2f getOrigin() const { return origin; }
    sf::Vector2f getHitPoint() const { return hitPoint; }
    
    void cast(float angle, sf::Vector2f startPoint, const Maze* maze);
    bool didHit() const { return rayLength < RAY_MAX_DISTANCE; }
    
    void reset() {
        rayLength = 0.0f;
        hitPoint = origin;
    }
};

#endif // RAY_H