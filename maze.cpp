#include "maze.h"
#include <random>
#include <iostream>
#include "SFML/Graphics.hpp"
using namespace std;


void update(int arr[2*N-1][2*N-1]){
    int temp[N-1][N-1];
    for(int i = 0; i < N-1; i++){
        for(int j = 0; j < N-1; j++){
            if(i == N-2)temp[i][j] = 2;
            else if(j == N-2)temp[i][j] = 1;
            else if(rand()%2){
                temp[i][j] = 1;
            }
            else temp[i][j] = 2;
        }
    }
    for(int i = 0; i < 2*N-1; i++){
        for(int j = 0; j < 2*N-1; j++){
            if(i == 0 || j == 0 || i == 2*N-2 || j == 2*N-2)arr[i][j] = 0;
            else if(i%2 && j%2) arr[i][j] = 1;
            else arr[i][j] = 0;
        }
    }
    for(int i = 0; i < N-1; i++){
        for(int j = 0; j < N-1; j++){
            if(i == N-2 && j == N-2)continue;
            if(temp[i][j] == 1){
                arr[2*i+2][2*j+1] = 1;
            }
            else arr[2*i+1][2*j+2] = 1;
        }
    }
}

Maze::Maze(){
    updateMaze();
}
void Maze::draw(sf::RenderWindow& w) const{
    sf::RectangleShape rec(sf::Vector2f(CUBE, CUBE));
    rec.setFillColor(sf::Color::Green);
    for(int i = 0; i < 2*N-1; i++){
        for(int j = 0; j < 2*N-1; j++){
            if(!this->arr[i][j]){
                rec.setPosition(CUBE*j, CUBE*i);
                w.draw(rec);
            }
        }
    }

}
bool Maze::getCell(int i, int j) const{
    return this->arr[i][j];
}
void Maze::updateMaze(){
    update(this->arr);
}
void Maze::showMat() const{
    for(int i = 0; i < 2*N-1; i++){
        for(int j = 0; j < 2*N-1; j++){
            cout << arr[i][j] << "  ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}