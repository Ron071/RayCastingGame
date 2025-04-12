#ifndef PLAYER_H
#define PLAYER_H

#include "ray.h"
#include "maze.h"
#include "SFML/Graphics.hpp"
#include <pthread.h>
using namespace sf;

struct Data {   
    int startIndex;
    int endIndex;
    Maze* maze;
    Player* player;
    Data(int start, int end, Maze* maze, Player* player)
        : startIndex(start), endIndex(end), maze(maze), player(player) {}
};



class Player{
    CircleShape player;
    std::vector<Ray> rays;
    pthread_t threads[THREAD_COUNT];
    std::vector<Data*> threadData;
    Maze* maze;
    public:
        Player(Maze* maze);
        void draw(RenderWindow* windowOne);
        float rotation();
        Vector2f position();
        Ray* ray(int i);
        bool checkCollision() const;
        void move();
        void turnR();
        void turnL();
        void reset();
        ~Player();
};

#endif //PLAYER_H