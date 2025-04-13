#ifndef MAZE_H
#define MAZE_H
#include "SFML/Graphics.hpp"
#define N 40
#define PI 3.142
#define CUBE 3//(float)285/(N*2-1)
using namespace std;

class Maze{
    int arr[2*N-1][2*N-1];
    public:
        Maze();
        void draw(sf::RenderWindow* w) const;
        int getCell(int i, int j) const;
        void updateMaze();
        void showMat() const;
};




#endif //MAZE_H