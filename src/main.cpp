#include "engine.hpp"
#include "entity.hpp"

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
    ecs.create_entity();
    ecs.add_component(Player, Name{"Player"});
    ecs.add_component(Player, Position{MAP_WIDTH / 2, MAP_HEIGHT / 2});
    ecs.add_component(Player, Renderable(PlayerMaleStanding));
    ecs.add_component(Player, Movement{0, 0});
    ecs.add_component(Player, Health{100, 100});
    ecs.add_component(Player, Damage{10, 10});

    EntityGeneratorSystem(ecs);

    // Create Map
    Grid grid = Grid{};
    MapGeneratorSystem(grid);
    BuildingGeneratorSystem(grid);

    bool wait = false;
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
                    SwapTilesetSystem(camera, grid, ecs);
                    continue;
                }
            } else if (event->is<sf::Event::Resized>()) {
                ResizeSystem(camera, grid, ecs);
                DrawSystem(window, renderTexture, camera, grid, ecs);
                wait = true;
            }

            if (!wait) {
                AIMovementIntentSystem(grid, ecs);
                InputSystem(ecs);
                CombatSystem(ecs);
                Health* hp = ecs.get_component<Health>(Player);
                if (hp->curr <= 0) {
                    printf("YOU DIED!\nBYE!\n");
                    return 0;
                }
                MovementSystem(grid, ecs);
                CameraSystem(camera, ecs);

                for (Entity entity : ecs.entities()) {
                    Movement* mov = ecs.get_component<Movement>(entity);

                    mov->dx = 0;
                    mov->dy = 0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        if (!wait) {
            DrawSystem(window, renderTexture, camera, grid, ecs);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            wait = true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
