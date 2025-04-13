#include "ray.h"
#include "player.h"
#include "maze.h"
#include <iostream>
#include <cmath>
using namespace std;

Ray::Ray():ray(sf::PrimitiveType::Lines, 2){}
void Ray::setPoints(Vector2f one, Vector2f two){
    ray[0].position = one;
    ray[1].position = two;
}
void Ray::draw(RenderWindow* windowOne) const{
    windowOne->draw(ray);
}
float Ray::len(){
    return lenght;
}
Vector2f Ray::finalPoint(float angle, const Maze* maze,  Vector2f current){  
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
        float z1 = abs(tempy/(sin(angle)));
        float z2 = abs(tempx/(cos(angle)));
        float t = fmin(z1, z2)+0.001;
        len += t-0.001;
        current+=direction*(t);
        if(len >= RAY_LEN){
            len = RAY_LEN;
            break;
        }
        if(!(maze->getCell((int)current.y/CUBE, (int)current.x/CUBE))){
            break;
        }
    }
    res += direction*len;
    lenght = len;
    return res;
}

