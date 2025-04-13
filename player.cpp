#include "player.h"
#include <iostream>
#include <unistd.h>
#define RADIANS(x) (((float)x/180*PI))

#define SW 285
#define BW 600



pthread_mutex_t lock;

Player::Player():player(CircleShape(2,10)){
    player.setPosition(sf::Vector2f(CUBE*1.5,CUBE*1.5));
    player.setOrigin(sf::Vector2f(2,2));
    for(int i = 0; i < RAYS; i++)rays.push_back(Ray());
}
float Player::rotation(){
    return player.getRotation().asRadians();  // [0 2PI]
}
Vector2f Player::position(){
    return player.getPosition();
}
Ray* Player::ray(int i){
    return &rays[i];
}
Player::~Player(){
}
// struct Data {
//     int startIndex;
//     int endIndex;
//     const Maze* maze;
//     Player* player;

//     Data(int start, int end, const Maze* maze, Player* player)
//         : startIndex(start), endIndex(end), maze(maze), player(player) {}
// };
// void* processRays(void* args) {
//     Data* data = static_cast<Data*>(args);
//     for (int i = data->startIndex; i < data->endIndex; ++i) {
//         float direction = atan((float)2 * (i - RAYS / 2) / RAYS) + data->player->rotation();
//         if (direction > 2 * PI) direction -= 2 * PI;
//         else if (direction < 0) direction += 2 * PI;
//         data->player->ray(i)->setPoints(data->player->position(),
//             data->player->ray(i)->finalPoint(direction, data->maze, data->player->position())
//         );
//     }
//     return nullptr;
// }
// void Player::draw(RenderWindow* windowOne, RenderWindow* windowTwo, const Maze* maze) {   ///100k 40fps 
//     windowOne->draw(player);
//     const int THREAD_COUNT = 6 < RAYS ? 6:RAYS; // to class 
//     pthread_t threads[THREAD_COUNT];
//     std::vector<Data*> threadData;
//     std::vector<RectangleShape*> shapes;
//     int raysPerThread = RAYS / THREAD_COUNT;
//     for (int t = 0; t < THREAD_COUNT; ++t) {
//         int startIndex = t * raysPerThread;
//         threadData.push_back(new Data(startIndex, startIndex + raysPerThread, maze, this));

//         pthread_create(&threads[t], nullptr, processRays, threadData[t]);
//     }
//     for (int t = 0; t < THREAD_COUNT; ++t) {
//         pthread_join(threads[t], nullptr);
//         delete threadData[t];  // in destructor 
//     }
//     for (int t = 0; t < RAYS; ++t){
//         rays[t].draw(windowOne);
//     }
// }

void Player::draw(RenderWindow* window, const Maze* maze){
        Vector2f current = this->position();
        float rotation = this->rotation();
        for(int i = 0; i <= RAYS; i++){
            float direction = atan(2*(float)(i-RAYS/2)/RAYS);
            float cosAngle = cos(direction);
            direction += rotation;
            if(direction > 2*PI)direction -= 2*PI;
            rays[i].draw(direction, maze, current, window); // add false to not draw lines 
            float height =(float)BW*CUBE/rays[i].len()/cosAngle;
            RectangleShape shape(Vector2f((float)BW/RAYS, height));
            shape.setFillColor(Color(0,256*(1-(float)rays[i].len()/RAY_LEN), 0));
            shape.setPosition(Vector2f(i*(float)BW/RAYS, 0.5*(BW-height)));
            window->draw(shape);
        }
        maze->draw(window);
        window->draw(player);
}
bool Player::checkCollision(const Maze& maze){
    Vector2f willBe = player.getPosition() + Vector2f(cos(this->rotation()), sin(this->rotation()));
    return !(maze.getCell((int)willBe.y/CUBE,(int)willBe.x/CUBE));
}
void Player::move(const Maze& maze){
    if(!checkCollision(maze)){
        player.move(Vector2f(SPEED*cos(this->rotation()), SPEED*sin(this->rotation())));
    }
}
void Player::turnR(){
    player.rotate(sf::degrees(6*SPEED));
}
void Player::turnL(){
    player.rotate(sf::degrees(-6*SPEED));
}
void Player::reset(){
    player.setPosition(Vector2f(CUBE*1.5,CUBE*1.5));
}
