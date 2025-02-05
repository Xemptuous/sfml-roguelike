#include "engine.hpp"

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <string.h>
#include <thread>

extern const int RENDER_WIDTH   = 2560;
extern const int RENDER_HEIGHT  = 1440;
extern const int CONSOLE_WIDTH  = 80;
extern const int CONSOLE_HEIGHT = 45;
extern const int MAP_WIDTH, MAP_HEIGHT;

// extern std::unordered_map<Entity, Position> positions;
// extern std::unordered_map<Entity, Movement> movements;
// extern std::unordered_map<Entity, Renderable> renderables;

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
    // CLI Args
    parseArgs(argc, argv);

    // Window setup
    sf::RenderWindow window;
    sf::RenderTexture renderTexture({RENDER_WIDTH, RENDER_HEIGHT});

    window.create(
        sf::VideoMode({RENDER_WIDTH, RENDER_HEIGHT}), "My Window", sf::Style::Default,
        sf::State::Windowed
    );
    window.setFramerateLimit(TARGET_FRAMERATE);

    Camera camera = Camera{.view = sf::View()};

    // Sprite Setup
    if (auto error_code = SpritesheetLoadingSystem() != 0) {
        return error_code;
    }
    SpriteGenerator();

    // Create Entities
    ECS ecs = ECS{};

    Entity player = ecs.create_entity();
    ecs.add_component(player, Position{MAP_WIDTH / 2, MAP_HEIGHT / 2});
    ecs.add_component(player, Renderable(PlayerMaleStanding));
    ecs.add_component(player, Movement{0, 0});

    // EntityGeneratorSystem(entityManager);

    // Create Map
    Grid grid = Grid{};
    MapGeneratorSystem(grid);
    BuildingGeneratorSystem(grid);

    bool wait = false;
    printf("DRAW SYSTEM\n");
    DrawSystem(window, renderTexture, camera, grid, ecs);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                window.close();
            } else if (event->is<sf::Event::KeyPressed>()) {
                wait = false;
                using namespace sf::Keyboard;
                if (isKeyPressed(Key::A)) {
                    SwapTilesetSystem(player, camera, grid, ecs);
                    continue;
                }
            } else if (event->is<sf::Event::Resized>()) {
                printf("RESIZE SYSTEM\n");
                ResizeSystem(player, camera, grid, ecs);
                wait = false;
            }

            if (!wait) {
                printf("INPUT SYSTEM\n");
                InputSystem(player, ecs);
                printf("MOVEMENT SYSTEM\n");
                MovementSystem(ecs);
                printf("COLLISION SYSTEM\n");
                CollisionSystem(grid, ecs);
                printf("CAMERA SYSTEM\n");
                CameraSystem(player, camera, ecs);
                printf("REMOVE MOVEMENTS\n");
                for (Entity entity : ecs.entities()) {
                    Movement* mov = ecs.get_component<Movement>(entity);
                    mov->dx       = 0;
                    mov->dy       = 0;
                }
            }
        }

        if (!wait) {
            printf("DRAW SYSTEM\n");
            DrawSystem(window, renderTexture, camera, grid, ecs);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
