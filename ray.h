#ifndef RAY_H
#define RAY_H

#include "maze.h"
#include <SFML/Graphics.hpp>

class Ray {
private:
    sf::Vector2f origin;
    sf::Vector2f hitPoint;
    float rayLength;
    float maxDistance;

public:
    Ray() : maxDistance(8.0f) {}
    explicit Ray(sf::Vector2f startOrigin, float maxDist = 8.0f)
        : origin(startOrigin), hitPoint(startOrigin), rayLength(0.0f), maxDistance(maxDist) {}

    void setMaxDistance(float dist) { maxDistance = dist; }
    float getLength() const { return rayLength; }
    sf::Vector2f getOrigin() const { return origin; }
    sf::Vector2f getHitPoint() const { return hitPoint; }

    void cast(float angle, sf::Vector2f startPoint, const Maze* maze);
    bool didHit() const { return rayLength < maxDistance; }

    void reset() {
        rayLength = 0.0f;
        hitPoint = origin;
    }
};

#endif // RAY_H