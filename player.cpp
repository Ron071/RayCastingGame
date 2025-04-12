#include "player.h"
#include <iostream>
#include <unistd.h>

/////getRotation return in degreee but all trigonomic functions expext to get in radians 


pthread_mutex_t lock;

Player::Player(Maze* maze):player(CircleShape(3,10)), maze(maze){
    player.setPosition(CUBE*1.5,CUBE*1.5);
    player.setOrigin(3,3);
    for(int i = 0; i < RAYS; i++)rays.push_back(Ray());
    int raysPerThread = RAYS / THREAD_COUNT;
    for (int t = 0; t < THREAD_COUNT; ++t) {
        int startIndex = t * raysPerThread;
        threadData.push_back(new Data(startIndex, startIndex + raysPerThread, maze, this));
    }
    pthread_mutex_init(&lock, nullptr);
}
float Player::rotation(){
    return RADIANS(player.getRotation());
}
Vector2f Player::position(){
    return player.getPosition();
}
Ray* Player::ray(int i){
    return &rays[i];
}
Player::~Player(){
    pthread_mutex_destroy(&lock);
    for (int t = 0; t < THREAD_COUNT; ++t){
        delete threadData[t];
    }
}

void* processRays(void* args) {
    Data* data = static_cast<Data*>(args);
    for (int i = data->startIndex; i < data->endIndex; ++i) {
        float direction = atan((float)2 * (i - RAYS / 2) / RAYS) + data->player->rotation();
        if (direction > 2 * PI) direction -= 2 * PI;
        else if (direction < 0) direction += 2 * PI;
        data->player->ray(i)->finalPoint(direction, data->maze, data->player->position());
        float height =(float)BW*CUBE/data->player->ray(i)->len()/cos(direction);
        RectangleShape shape(Vector2f((float)BW/RAYS, height));
        shape.setFillColor(Color(0,256*(1-(float)data->player->ray(i)->len()/RAY_LEN), 0));
        shape.setPosition(i*(float)BW/RAYS, 0.5*(BW-height));
        pthread_mutex_lock(&lock);
        //window->draw(shape); /// NEED TO THINK
        pthread_mutex_unlock(&lock);
    }
    return nullptr;
}
void Player::draw(RenderWindow* window) {  
    Vector2f current = player.getPosition();
    int raysPerThread = RAYS / THREAD_COUNT;
    for (int t = 0; t < THREAD_COUNT; ++t) {
        pthread_create(&threads[t], nullptr, processRays, threadData[t]);
    }
    for (int t = 0; t < THREAD_COUNT; ++t) {
        pthread_join(threads[t], nullptr);
    }
    
}

// void Player::draw(RenderWindow* window, const Maze* maze){/// 100k 22fps 
//         Vector2f current = player.getPosition();
//         float rotation = RADIANS(player.getRotation());
//         for(int i = 0; i < RAYS; i++){
//             float direction = atan((float)2*(i-RAYS/2)/RAYS);// make it depend on the ANGLE 
//             float cosAngle = cos(direction);
//             direction += rotation;
//             if(direction > 2*PI)direction -= 2*PI;
//             else if (direction < 0)direction += 2*PI;
//             rays[i].finalPoint(direction, maze, current);
//             //rays[i].setPoints(current, rays[i].finalPoint(direction, maze, current));
//             //rays[i].draw(windowOne);
//             float height =(float)BW*CUBE/rays[i].len()/cosAngle;
//             RectangleShape shape(Vector2f((float)BW/RAYS, height));
//             shape.setFillColor(Color(0,256*(1-(float)rays[i].len()/RAY_LEN), 0));
//             shape.setPosition(i*(float)BW/RAYS, 0.5*(BW-height));
//             window->draw(shape);
//         }
//         maze->draw(window);
//         CircleShape temp = player;
//         temp.move(0,(BW-2*N*CUBE));
//         window->draw(temp);
// }
    bool Player::checkCollision() const {
        Vector2f willBe = player.getPosition() + Vector2f(cos(RADIANS(player.getRotation())), sin(RADIANS(player.getRotation())));
        return !(maze->getCell((int)willBe.y/CUBE,(int)willBe.x/CUBE));
    }
    void Player::move(){
        if(!checkCollision()){
            player.move(SPEED*cos(RADIANS(player.getRotation())), SPEED*sin(RADIANS(player.getRotation())));
        }
    }
    void Player::turnR(){
        player.rotate(3);
    }
    void Player::turnL(){
        player.rotate(-3);
    }
    void Player::reset(){
        player.setPosition(CUBE*1.5,CUBE*1.5);
    }
