#include "config.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace {
    // Menu color scheme
    constexpr sf::Color BG_COLOR(20, 20, 30);
    constexpr sf::Color TEXT_COLOR(220, 220, 220);
    constexpr sf::Color HIGHLIGHT_COLOR(100, 200, 100);
    constexpr sf::Color TITLE_COLOR(100, 180, 255);
    constexpr float FONT_SIZE = 20.0f;
    constexpr float LINE_HEIGHT = 35.0f;
}

// Helper to clamp a value between min and max
template<typename T>
T clamp(T value, T minVal, T maxVal) {
    return std::max(minVal, std::min(maxVal, value));
}

// Draw the configuration menu
void drawConfigMenu(sf::RenderWindow& window, const sf::Font& font,
                    const GameConfig& config, int selectedIndex,
                    const std::vector<std::string>& labels,
                    const std::vector<std::string>& values) {
    window.clear(BG_COLOR);

    // Title
    sf::Text title(font, "RAYCASTING MAZE GAME", 36);
    title.setFillColor(TITLE_COLOR);
    title.setPosition({static_cast<float>(config.windowSize) / 2 - title.getGlobalBounds().size.x / 2, 30});
    window.draw(title);

    sf::Text subtitle(font, "Configuration Menu", 24);
    subtitle.setFillColor(TITLE_COLOR);
    subtitle.setPosition({static_cast<float>(config.windowSize) / 2 - subtitle.getGlobalBounds().size.x / 2, 80});
    window.draw(subtitle);

    // Settings list
    for (size_t i = 0; i < labels.size(); ++i) {
        float y = 140.0f + i * LINE_HEIGHT;
        bool isSelected = (static_cast<int>(i) == selectedIndex);

        // Background for selected item
        if (isSelected) {
            sf::RectangleShape highlight(
                sf::Vector2f(static_cast<float>(config.windowSize) - 60, LINE_HEIGHT - 5));
            highlight.setPosition({30.0f, y + 2});
            highlight.setFillColor(sf::Color(40, 60, 40));
            window.draw(highlight);
        }

        // Label
        sf::Text label(font, labels[i], FONT_SIZE);
        label.setFillColor(isSelected ? HIGHLIGHT_COLOR : TEXT_COLOR);
        label.setPosition({40.0f, y + 5});
        window.draw(label);

        // Value
        sf::Text value(font, values[i], FONT_SIZE);
        value.setFillColor(isSelected ? HIGHLIGHT_COLOR : TEXT_COLOR);
        value.setPosition({static_cast<float>(config.windowSize) - value.getGlobalBounds().size.x - 40, y + 5});
        window.draw(value);
    }

    // Instructions
    sf::Text instructions(font, "UP/DOWN: Navigate  |  LEFT/RIGHT: Change  |  ENTER: Start  |  ESC: Defaults", 16);
    instructions.setFillColor(sf::Color(150, 150, 150));
    instructions.setPosition({static_cast<float>(config.windowSize) / 2 - instructions.getGlobalBounds().size.x / 2,
                               static_cast<float>(config.windowSize) - 40});
    window.draw(instructions);

    window.display();
}

GameConfig showConfigMenu() {
    GameConfig config;

    // Create window for config menu
    sf::RenderWindow window(
        sf::VideoMode({static_cast<unsigned int>(config.windowSize),
                       static_cast<unsigned int>(config.windowSize)}),
        "Raycasting Maze Game - Configuration",
        sf::Style::Titlebar | sf::Style::Close);

    window.setFramerateLimit(30);

    // Load font
    sf::Font font;
    const char* fontPaths[] = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "C:\\Windows\\Fonts\\arial.ttf"
    };
    for (const char* path : fontPaths) {
        if (font.openFromFile(path)) break;
    }

    // Configuration options
    enum ConfigOption {
        OPT_MAZE_SIZE,
        OPT_RAY_COUNT,
        OPT_RAY_DISTANCE,
        OPT_THREAD_COUNT,
        OPT_GOALS_TO_WIN,
        OPT_PLAYER_SPEED,
        OPT_COUNT
    };

    int selectedIndex = 0;
    bool running = true;

    while (running && window.isOpen()) {
        // Build labels and values
        std::vector<std::string> labels = {
            "Maze Size:",
            "Ray Count:",
            "Ray Max Distance:",
            "Thread Count:",
            "Goals to Win:",
            "Player Speed:"
        };

        std::vector<std::string> values = {
            std::to_string(config.mazeSize) + "x" + std::to_string(config.mazeSize),
            std::to_string(config.rayCount),
            std::to_string(static_cast<int>(config.rayMaxDistance)),
            std::to_string(config.threadCount),
            std::to_string(config.goalsToWin),
            std::to_string(config.playerSpeed)
        };

        // Event handling
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>() ||
                (event->is<sf::Event::KeyPressed>() &&
                 event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape)) {
                // ESC resets to defaults and starts
                config = GameConfig();
                running = false;
            }

            if (event->is<sf::Event::KeyPressed>()) {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (!keyEvent) continue;

                switch (keyEvent->code) {
                    case sf::Keyboard::Key::Up:
                        selectedIndex = (selectedIndex - 1 + OPT_COUNT) % OPT_COUNT;
                        break;
                    case sf::Keyboard::Key::Down:
                        selectedIndex = (selectedIndex + 1) % OPT_COUNT;
                        break;
                    case sf::Keyboard::Key::Enter:
                        running = false;
                        break;
                    case sf::Keyboard::Key::Left:
                    case sf::Keyboard::Key::Right: {
                        int delta = (keyEvent->code == sf::Keyboard::Key::Right) ? 1 : -1;
                        switch (selectedIndex) {
                            case OPT_MAZE_SIZE:
                                config.mazeSize = clamp(config.mazeSize + delta * 2, 11, 51);
                                // Keep maze size odd
                                if (config.mazeSize % 2 == 0) config.mazeSize += delta;
                                break;
                            case OPT_RAY_COUNT:
                                config.rayCount = clamp(config.rayCount + delta * 50, 100, 2000);
                                break;
                            case OPT_RAY_DISTANCE:
                                config.rayMaxDistance = clamp(config.rayMaxDistance + delta, 4.0f, 20.0f);
                                break;
                            case OPT_THREAD_COUNT:
                                config.threadCount = clamp(config.threadCount + delta, 1, 16);
                                break;
                            case OPT_GOALS_TO_WIN:
                                config.goalsToWin = clamp(config.goalsToWin + delta, 1, 10);
                                break;
                            case OPT_PLAYER_SPEED: {
                                float newSpeed = config.playerSpeed + delta * 0.01f;
                                config.playerSpeed = clamp(newSpeed, 0.02f, 0.15f);
                                break;
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        drawConfigMenu(window, font, config, selectedIndex, labels, values);
    }

    window.close();
    return config;
}