#include "engine.hpp"
#include "grid.hpp"
#include "sprite.hpp"

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <string.h>
#include <thread>

extern const int RENDER_WIDTH   = 2560;
extern const int RENDER_HEIGHT  = 1440;
extern const int CONSOLE_WIDTH  = 80;
extern const int CONSOLE_HEIGHT = 45;
extern const int MAP_WIDTH, MAP_HEIGHT;

extern std::unordered_map<Entity, Position> positions;
extern std::unordered_map<Entity, Movement> movements;
extern std::unordered_map<Entity, Renderable> renderables;

sf::Texture SPRITE_SHEET;
sf::Image SPRITE_SHEET_IMAGE;
extern Options OPTIONS;

const int TARGET_FRAMERATE = 24;

void parseArgs(int argc, char** argv) {
    bool is_ascii = false;
    for (int i = 1; i < argc; i++) {
        is_ascii = strcmp(argv[i], "--sprites") == 0;
    }
    OPTIONS.is_ascii = true;
}

int main(int argc, char** argv) {
    // Window setup
    sf::RenderWindow window;
    sf::RenderTexture renderTexture({RENDER_WIDTH, RENDER_HEIGHT});
    sf::View view;
    sf::Font font;

    window.create(
        sf::VideoMode({RENDER_WIDTH, RENDER_HEIGHT}), "My Window", sf::Style::Default,
        sf::State::Windowed
    );
    window.setFramerateLimit(TARGET_FRAMERATE);

    // CLI Args
    parseArgs(argc, argv);

    if (auto error_code = SpritesheetLoadingSystem(OPTIONS.is_ascii) != 0) {
        return error_code;
    }

    // World Setup
    SpriteGenerator(OPTIONS.is_ascii);

    Entity player = 1;

    positions[player]   = {MAP_WIDTH / 2, MAP_HEIGHT / 2};
    renderables[player] = Renderable(PlayerMaleStanding);
    std::vector<Entity> entities;
    entities.push_back(player);

    Grid grid = Grid();
    MapGeneratorSystem(grid);
    BuildingGeneratorSystem(grid);
    Camera camera = Camera{.view = view};

    bool wait = false;
    DrawSystem(window, renderTexture, camera, grid);
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                window.close();
            } else if (event->is<sf::Event::KeyPressed>()) {
                wait = false;
                using namespace sf::Keyboard;
                // InputSystem(player);
                if (isKeyPressed(Key::A)) {
                    SwapTilesetSystem(player, camera, grid);
                    continue;
                }
            } else if (event->is<sf::Event::Resized>()) {
                ResizeSystem(player, camera, grid);
                wait = false;
            }

            if (!wait) {
                InputSystem(player);
                MovementSystem(entities);
                CollisionSystem(grid);
                CameraSystem(player, camera);
                movements.clear();
            }
        }

        if (!wait) {
            DrawSystem(window, renderTexture, camera, grid);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
