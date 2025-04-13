#include "game.h"
#include <iostream>
int main(){
    Game g;
    g.game();
    return 0;

}
/*
g++ -std=c++23 main.cpp game.cpp maze.cpp player.cpp ray.cpp -I /Users/ronaldgrinberg/Desktop/SFML-3.0.0/include -L /Users/ronaldgrinberg/Desktop/SFML-3.0.0/lib -lsfml-graphics -lsfml-window -lsfml-system -pthread -o game -Wl,-rpath,/Users/ronaldgrinberg/Desktop/SFML-3.0.0/lib
    */