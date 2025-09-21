#ifndef RAY_H
#define RAY_H

#include "maze.h"
#define RAY_LEN 8

using namespace sf;

/**
 * Represents a single ray used for raycasting.
 * Each ray has a start, an end, and a computed length
 * based on collision with the maze walls.
 */
class Ray {
    Vector2f start;
    Vector2f end;
    float length;

public:
    Ray() = default;
    float len() const;  
    void renderRay(float angle, Vector2f startPoint, const Maze* maze);
};

#endif // RAY_H
