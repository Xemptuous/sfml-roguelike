#include "player.hpp"

#include "sprite.hpp"

#include <cmath>

extern const int RENDER_WIDTH, RENDER_HEIGHT;
extern const int CONSOLE_WIDTH, CONSOLE_HEIGHT;
extern const int MAP_WIDTH, MAP_HEIGHT;

extern sf::RenderWindow window;
extern sf::RenderTexture renderTexture;

Player::Player(SpriteTiles tile) {
    auto [w, h]    = window.getSize();
    this->sprite   = getSpriteTile(tile);
    this->position = {MAP_WIDTH / 2, MAP_HEIGHT / 2};
}
