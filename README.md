# Raycasting Maze Game

A real-time 3D maze game implemented in C++ using SFML with multithreaded raycasting.

## Features

- Procedural maze generation
- Multithreaded raycasting using POSIX threads
- Thread-safe rendering with mutexes
- Real-time player controls and collision detection
- Timer and score system
- Minimap view

## Controls

| Key | Action |
|-----|--------|
| W | Move forward |
| A | Rotate left |
| D | Rotate right |
| U | Regenerate maze |

## Build

```bash
g++ -g -std=c++23 -o game main.cpp game.cpp maze.cpp player.cpp ray.cpp \
    -I./SFML/include -L./SFML/lib \
    -lsfml-graphics -lsfml-window -lsfml-system -pthread \
    -Wl,-rpath,@loader_path/SFML/lib
```

## Run

```bash
./game
```

## Project Structure

- `main.cpp` - Entry point
- `game.h/.cpp` - Main game class
- `maze.h/.cpp` - Maze generation and rendering
- `player.h/.cpp` - Player movement and raycasting
- `ray.h/.cpp` - Ray casting algorithm

## Changes Summary

### Naming Improvements
- `NUMBER` → `MAZE_SIZE`
- `SW` → `MINIMAP_SIZE`
- `BW` → `WINDOW_SIZE`
- `arr` → `mazeData`
- `turnL()` → `rotateLeft()`
- `turnR()` → `rotateRight()`
- `len()` → `getLength()`
- `renderRay()` → `cast()`

### Design Improvements
- Added `enum class MazeCell` for type-safe cell types
- Private members with public getters
- Separated game logic into handleEvents, update, render
- Thread data structure for cleaner communication
- Cross-platform font loading
- Error handling in main