#pragma once
#include "sprite.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window.hpp>

using namespace sf;

struct Player {
    Sprite* sprite;
    // SpriteTiles sprite_type;
    Vector2i position;

    Player(SpriteTiles tile);
    // void draw(sf::Vector, SizeFactor);
};
