#ifndef MAZE_H
#define MAZE_H

#define NUMBER 20
#define PI 3.142
#define CUBE 7 // 285/(NUMBER*2-1)
#include "SFML/Graphics.hpp"
using namespace std;

class Maze{
    int arr[2*NUMBER-1][2*NUMBER-1];
    public:
        Maze();
        void draw(sf::RenderWindow* w) const;
        int getCell(int i, int j) const;
        void updateMaze();
        void showMat() const;
};




#endif //MAZE_H