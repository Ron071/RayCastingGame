#include "game.h"
#include <iostream>
#include <SFML/Audio.hpp>


Game::Game():player(&maze){}

void Game::game(){
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; 
    sf::RenderWindow window(sf::VideoMode(BW, BW), "GAME", sf::Style::Default, settings);
    window.setFramerateLimit(30);
    // sf::Clock clock; 
    // sf::Time deltaTime;
    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
                exit(0);
            }    
        }
        if(Keyboard::isKeyPressed(Keyboard::W)){
            player.move();
        }
        if(Keyboard::isKeyPressed(Keyboard::A)){
            player.turnL();
        }
        if(Keyboard::isKeyPressed(Keyboard::D)){
            player.turnR();
        }
        window.clear();
        player.draw(&window);
        window.display();  
        // avr += (int)(1.0f / deltaTime.asSeconds());
        // count++;
        // if(count%500 == 0){
        //     std::cout <<  "FPS " << avr/count << std::endl;
        //     count = 0;
        //     avr = 0;
        // }
        // deltaTime = clock.restart();
    }
}
void Game::update(){
    player.reset();
    maze.updateMaze();
}
////   g++ -g game.cpp main.cpp maze.cpp player.cpp ray.cpp  -lsfml-graphics -lsfml-window -lsfml-system -pthread -o game
