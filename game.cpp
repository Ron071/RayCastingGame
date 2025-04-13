#include "game.h"
#include <iostream>
#include "SFML/Audio.hpp"


Game::Game(){  
}
void Game::game(){
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(Vector2u(BW,BW)), "GAME");
    window.setFramerateLimit(60);
    // sf::Clock clock; 
    // sf::Time deltaTime;
    while(window.isOpen()){
        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
                exit(0);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            player.move(maze);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            player.turnL(); 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            player.turnR(); 
        }
        window.clear();
        player.draw(&window, &maze);
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




/*   g++ -g -std=c++23 main.cpp game.cpp maze.cpp player.cpp ray.cpp \
    -I /Users/ronaldgrinberg/Desktop/SFML-3.0.0/include \
    -L /Users/ronaldgrinberg/Desktop/SFML-3.0.0/lib \
    -lsfml-graphics -lsfml-window -lsfml-system -pthread \
    -o game -Wl,-rpath,/Users/ronaldgrinberg/Desktop/SFML-3.0.0/lib  
*/