#pragma once
#include "sprite.hpp"

#include <SFML/Graphics.hpp>

using namespace sf;

using Entity = std::uint64_t;

const Entity MAX_ENTITIES = 10000;

// Components
// struct Position {
//     int x, y;
// };
// struct Velocity {
//     int dx, dy;
// };

struct Player {
    Sprite sprite;
    SpriteTiles sprite_type;
    Vector2i position;

    Player(SpriteTiles tile);
};

// struct Monster {
//     Sprite* sprite;
//     Vector2i position;
// };
