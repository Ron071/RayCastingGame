#include "maze.h"
#include <random>
#include <iostream>
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
    while(1){
        int i = rand()%(2*N-1);
        int j = rand()%(2*N-1);
        if(arr[i][j] == 1){
            arr[i][j] = 2;
            return;
        }
    }

}

Maze::Maze(){
    updateMaze();
}
void Maze::draw(sf::RenderWindow* window) const{
    sf::RectangleShape cube(sf::Vector2f(CUBE, CUBE));
    cube.setFillColor(sf::Color::Blue);
    sf::RectangleShape cube2(sf::Vector2f(CUBE, CUBE));
    cube2.setFillColor(sf::Color::Black);
    for(int i = 0; i < 2*N-1; i++){
        for(int j = 0; j < 2*N-1; j++){
            if(this->arr[i][j] == 0){
                cube.setPosition(sf::Vector2f(CUBE*j, (600-2*N*CUBE) + CUBE*i));
                window->draw(cube);
            }
            else if(this->arr[i][j] == 2){
                cube2.setPosition(sf::Vector2f(CUBE*j, (600-2*N*CUBE)+ CUBE*i));
                window->draw(cube2);
            }
        }
    }

}
int Maze::getCell(int i, int j) const{
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