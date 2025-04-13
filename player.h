#ifndef PLAYER_H
#define PLAYER_H

#include "ray.h"
#include "maze.h"
#include "SFML/Graphics.hpp"
#include <pthread.h>
using namespace sf;
#define SPEED 0.1
#define RAYS 1000
#define ANGLE 90




class Player{
    CircleShape player;
    std::vector<Ray> rays;
    public:
        Player();
        void draw(RenderWindow* windowOne, const Maze* maze);
        float rotation();
        Vector2f position();
        Ray* ray(int i);
        bool checkCollision(const Maze& maze);
        void move(const Maze& maze);
        void turnR();
        void turnL();
        void reset();
        ~Player();
};

#endif //PLAYER_H