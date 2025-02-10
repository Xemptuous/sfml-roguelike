#include "engine.hpp"
#include "entity.hpp"
#include "sprite.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstdio>
#include <string.h>
#include <thread>

extern constexpr int RENDER_WIDTH   = 2560;
extern constexpr int RENDER_HEIGHT  = 1440;
extern constexpr int CONSOLE_WIDTH  = 80;
extern constexpr int CONSOLE_HEIGHT = 45;

// extern std::unordered_map<Entity, Position> positions;
// extern std::unordered_map<Entity, Movement> movements;
// extern std::unordered_map<Entity, Renderable> renderables;

sf::Texture SPRITE_SHEET;
sf::Image SPRITE_SHEET_IMAGE;
extern Options OPTIONS;

extern MenuOptions MENU_OPTIONS;

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
    sf::Font font;
    window.create(
        sf::VideoMode({RENDER_WIDTH, RENDER_HEIGHT}), "My Window", sf::Style::Default,
        sf::State::Windowed
    );
    window.setFramerateLimit(TARGET_FRAMERATE);
    if (!font.openFromFile("../include/DejaVuSans.ttf")) {
        printf("Could not load font file!\n");
        return 1;
    }

    Camera camera = Camera{.view = sf::View()};

    // Sprite Setup
    if (auto error_code = SpritesheetLoadingSystem() != 0) {
        return error_code;
    }
    SpriteGenerator();

    // Create Entities
    ECS ecs;
    Player = ecs.create_entity()
                 .with(Actor{})
                 .with(Name{"Player"})
                 .with(Position{MAP_WIDTH / 2, MAP_HEIGHT / 2})
                 .with(Renderable(PlayerMaleStanding))
                 .with(Movement{0, 0})
                 .with(Health{100, 100})
                 .with(Damage{10, 10})
                 .with(Inventory{})
                 .build();

    ItemRegistry itemRegistry;
    ItemGeneratorSystem(itemRegistry, ecs);
    EntityGeneratorSystem(ecs);

    printf("SIZE OF SPRITE: %lu\n", sizeof(sf::Sprite));
    printf("SIZE OF COLOR: %lu\n", sizeof(sf::Color));
    printf("SIZE OF VECTOR2i: %lu\n", sizeof(Vector2i));
    printf("SIZE OF TILE: %lu\n", sizeof(Tile));
    printf("SIZE OF ECS: %lu\n", sizeof(ECS));
    printf("SIZE OF EntityManager: %lu\n", sizeof(EntityManager));
    printf("SIZE OF ComponentManager: %lu\n", sizeof(ComponentManager));
    printf("SIZE OF GRID: %lu\n", sizeof(Grid));

    // Give player an item
    addItemToInventory(Player, itemRegistry.get("wood sword"), ecs);
    addItemToInventory(Player, itemRegistry.get("iron helmet"), ecs);

    // Create Map
    Grid grid{};
    MapGeneratorSystem(grid);
    BuildingGeneratorSystem(grid);

    bool wait = false;
    DrawSystem(window, renderTexture, font, camera, grid, ecs);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            using namespace sf::Keyboard;
            if (event->is<Event::Closed>() || isKeyPressed(Key::Q)) {
                window.close();
            } else if (event->is<Event::KeyPressed>()) {
                wait = false;
            } else if (event->is<Event::Resized>()) {
                ResizeSystem(camera, grid, ecs);
                DrawSystem(window, renderTexture, font, camera, grid, ecs);
                wait = true;
            }

            if (!wait) {
                if (isKeyPressed(Key::A)) {
                    SwapTilesetSystem(camera, grid, ecs);
                    continue;
                } else if (isKeyPressed(Key::I)) {
                    MENU_OPTIONS.show_inventory = true;
                    wait                        = true;
                } else if (isKeyPressed(Key::Escape)) {
                    MENU_OPTIONS.show_inventory = false;
                    wait                        = true;
                }
            }
        }

        if (!wait) {
            AIMovementIntentSystem(grid, ecs);
            InputSystem(ecs);
            CombatSystem(itemRegistry, ecs);
            Health* hp = ecs.get_component<Health>(Player);
            if (hp->curr <= 0) {
                printf("YOU DIED!\nBYE!\n");
                return 0;
            }
            MovementSystem(grid, ecs);
            CameraSystem(camera, ecs);

            for (Entity entity : ecs.entities()) {
                Movement* mov = ecs.get_component<Movement>(entity);
                if (!mov) continue;

                mov->dx = 0;
                mov->dy = 0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            wait = true;
        }
        DrawSystem(window, renderTexture, font, camera, grid, ecs);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
