#include "config.h"
#include "game.h"
#include <iostream>

int main() {
    try {
        // Show configuration menu
        GameConfig config = showConfigMenu();

        // Start game with selected configuration
        Game game(config);
        game.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}