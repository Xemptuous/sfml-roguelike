#include "engine.hpp"
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

const int TARGET_FRAMERATE = 24;

sf::RenderWindow window;
sf::RenderTexture renderTexture({RENDER_WIDTH, RENDER_HEIGHT});
sf::Texture SPRITE_SHEET;
sf::Image SPRITE_SHEET_IMAGE;
sf::Font font;
sf::View view;

void initWindow() {
    window.create(
        sf::VideoMode({RENDER_WIDTH, RENDER_HEIGHT}), "My Window", sf::Style::Default,
        sf::State::Windowed
    );
    window.setFramerateLimit(TARGET_FRAMERATE);
}

Options parseArgs(int argc, char** argv) {
    bool is_ascii = false;
    for (int i = 1; i < argc; i++) {
        is_ascii = strcmp(argv[i], "--sprites") == 0;
    }
    return Options{is_ascii};
}

void draw(Engine* engine) {
    window.clear();
    renderTexture.clear();

    engine->drawMap();
    engine->drawEntities();

    renderTexture.display();
    const sf::Texture& texture = renderTexture.getTexture();
    sf::Sprite sprite(texture);

    window.draw(sprite);
    window.display();
}

int main(int argc, char** argv) {
    Options options = parseArgs(argc, argv);

    initWindow();
    if (auto error_code = initSpriteSheet(options.is_ascii) != 0) {
        return error_code;
    }

    generateSpritesVector(options.is_ascii);
    std::unique_ptr<Engine> engine(new Engine(options));

    bool wait = false;
    draw(engine.get());
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                window.close();
            } else if (event->is<sf::Event::KeyPressed>()) {
                wait = false;
                using namespace sf::Keyboard;
                if (isKeyPressed(Key::A)) {
                    engine->swapTilesets();
                }
            } else if (event->is<sf::Event::Resized>()) {
                engine->update();
                wait = false;
            }

            if (!wait) {
                engine->handleKey();
            }
        }

        if (!wait) {
            renderTexture.setView(view);
            draw(engine.get());
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
