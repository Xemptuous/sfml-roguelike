#include "entity.hpp"

#include "sprite.hpp"

#include <cmath>

extern const int MAP_WIDTH, MAP_HEIGHT;

Player::Player(SpriteTiles tile)
    : sprite(*getSpriteTile(tile)), sprite_type(tile), position({MAP_WIDTH / 2, MAP_HEIGHT / 2}) {}
