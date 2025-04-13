#ifndef RAY_H
#define RAY_H
#include "maze.h"
#include "SFML/Graphics.hpp"
#define RAY_LEN 50
using namespace sf;


class Ray{
    Vector2f start;
    Vector2f end;
    float lenght;
    public:
        Ray();
        float len();
        void setPoints(Vector2f one, Vector2f two);
        Vector2f draw(float angle, const Maze* maze,  Vector2f current, RenderWindow* window, bool display = true);
};

#endif //RAY_H