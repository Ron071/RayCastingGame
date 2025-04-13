#include "ray.h"
#include "player.h"
#include "maze.h"
#include <iostream>
#include <cmath>
using namespace std;

Ray::Ray(){}
void Ray::setPoints(Vector2f one, Vector2f two){
    start = one;
    end = two;
}
float Ray::len(){
    return lenght;
}
Vector2f Ray::draw(float angle, const Maze* maze,  Vector2f current, RenderWindow* window, bool display){  
    float len = 0;
    Vector2f direction(std::cos(angle), std::sin(angle));
    Vector2f res = current;
    float tempx;
    float tempy;
    while(1){
        int currx = (int)current.x/CUBE;
        int curry = (int)current.y/CUBE;
        if(angle <= (PI/2) || angle >= (1.5*PI)){
            tempx = (currx+1)*(CUBE)-current.x;
        } 
        else{
        tempx = current.x - currx*CUBE;  
        } 
        if(angle < PI ){
            tempy = (curry +1)*(CUBE) - current.y;
        } 
        else {
            tempy = current.y - curry*CUBE;
        }
        float z1 = abs(tempy/(sin(angle) != 0 ? sin(angle) : 1e-6));
        float z2 = abs(tempx/(cos(angle) != 0 ? cos(angle) : 1e-6));
        float t = fmin(z1, z2)+0.01;
        len += t-0.01;
        current+=direction*(t);
        if(len >= RAY_LEN){
            len = RAY_LEN;
            break;
        }
        if(!(maze->getCell((int)current.y/CUBE, (int)current.x/CUBE))){
            break;
        }
    }
    current = res;
    res += direction*len;
    lenght = len;
    if(display){
        setPoints(current, res);
        std::array line ={ Vertex{start},Vertex{end}};
        window->draw(line.data(), line.size(), sf::PrimitiveType::Lines);
    }
    return res;
}

