#include "game.h"
#include <iostream>
#include <SFML/Audio.hpp>


Game::Game(){  
}
void Game::game(){
    sf::RenderWindow windowOne(VideoMode(SW, SW), "MAZE");
    sf::RenderWindow windowTwo(VideoMode(BW, BW), "GAME");
    windowTwo.setFramerateLimit(120);
    sf::Clock clock;  // set font inside class
    sf::Time deltaTime;
    sf::Font font;
    font.loadFromFile("text.ttf");
    sf::Text text;
    text.setFont(font);
    int i = 0;
    while(windowTwo.isOpen()){
        Event event;
        while(windowTwo.pollEvent(event)){
            if(event.type == Event::Closed){
                windowOne.close();
                windowTwo.close();
            }    
        }
        if(Keyboard::isKeyPressed(Keyboard::W)){
            player.move(maze);
        }
        if(Keyboard::isKeyPressed(Keyboard::A)){
            player.turnL();
        }
        if(Keyboard::isKeyPressed(Keyboard::D)){
            player.turnR();
        }
        if(Keyboard::isKeyPressed(Keyboard::U)){
            update();
        }
        deltaTime = clock.restart();
        windowOne.clear();
        windowTwo.clear();
        maze.draw(windowOne);
        player.draw(windowOne, windowTwo, maze);
        windowTwo.draw(text);
        if(i > 100){
            float fps = 1.0f / deltaTime.asSeconds();
            text.setString("FPS: " + std::to_string((int)round(fps / 10) * 10));
            text.setCharacterSize(35);
            text.setFillColor(sf::Color::Black);
            text.setStyle(sf::Text::Bold | sf::Text::Underlined);
            text.setPosition(Vector2f(10,10));
            i=0;
        }
        i++;
        windowTwo.display();  
        windowOne.display();
    }
}
void Game::update(){
    player.reset();
    maze.updateMaze();
}




////   g++ game.cpp main.cpp maze.cpp player.cpp ray.cpp  -lsfml-graphics -lsfml-window -lsfml-system