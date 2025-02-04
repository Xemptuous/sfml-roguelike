#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <unordered_map>
#include <vector>

extern sf::Texture SPRITE_SHEET;
extern sf::Image SPRITE_SHEET_IMAGE;

enum SpriteTiles : size_t {
    None,
    BrownWall1,
    BrownWall2,
    BrownWall3,
    BrownWallCracked1,
    BrownWallCracked2,
    BrownWallCracked3,
    WoodWall1,
    StoneWall1,
    DoorClosed1,
    Door2,
    Door3,
    Door4,
    Stone1,
    Stone2,
    Wood1,
    StoneWindow1,
    StoneWindow2,
    Carpet1,
    Carpet2,
    Carpet3,
    Carpet4,
    PlayerMaleStanding,
    PlayerMaleWalking1,
    PlayerMaleWalking2,
    Grass1,
    Grass2,
    DimGrass1,
    DimGrass2,
    Flower1,
    Flower2,
    Mushroom1,
    Mushroom2,
    Tree1,
    Tree2,
    COUNT
};

constexpr size_t NUM_SPRITES = static_cast<size_t>(SpriteTiles::COUNT);

static const std::unordered_map<SpriteTiles, sf::Vector2i> SPRITE_COORDINATES = {
    // (x, y) from upperleft
    {None,               sf::Vector2i({17, 0})},
    {BrownWall1,         sf::Vector2i({0, 0}) },
    {BrownWall2,         sf::Vector2i({1, 0}) },
    {BrownWall3,         sf::Vector2i({2, 0}) },
    {BrownWallCracked1,  sf::Vector2i({3, 0}) },
    {BrownWallCracked2,  sf::Vector2i({4, 0}) },
    {BrownWallCracked3,  sf::Vector2i({5, 0}) },
    {WoodWall1,          sf::Vector2i({2, 4}) },
    {StoneWall1,         sf::Vector2i({7, 2}) },
    {DoorClosed1,        sf::Vector2i({4, 1}) },
    {Door2,              sf::Vector2i({1, 1}) },
    {Door3,              sf::Vector2i({2, 1}) },
    {Door4,              sf::Vector2i({3, 1}) },
    {Stone1,             sf::Vector2i({0, 2}) },
    {Stone2,             sf::Vector2i({1, 2}) },
    {Wood1,              sf::Vector2i({2, 4}) },
    {StoneWindow1,       sf::Vector2i({3, 2}) },
    {StoneWindow2,       sf::Vector2i({4, 2}) },
    {Carpet1,            sf::Vector2i({4, 5}) },
    {Carpet2,            sf::Vector2i({2, 6}) },
    {Carpet3,            sf::Vector2i({8, 5}) },
    {Carpet4,            sf::Vector2i({8, 6}) },
    {PlayerMaleStanding, sf::Vector2i({0, 16})},
    {Grass1,             sf::Vector2i({3, 9}) },
    {Grass2,             sf::Vector2i({4, 9}) },
    {DimGrass1,          sf::Vector2i({15, 5})},
    {DimGrass2,          sf::Vector2i({16, 5})},
    {Flower1,            sf::Vector2i({0, 10})},
    {Flower2,            sf::Vector2i({1, 10})},
    {Mushroom1,          sf::Vector2i({2, 10})},
    {Mushroom2,          sf::Vector2i({3, 10})},
    {Tree1,              sf::Vector2i({4, 10})},
    {Tree2,              sf::Vector2i({5, 10})},
};

static const std::unordered_map<SpriteTiles, sf::Vector2i> ASCII_COORDINATES = {
    // (x, y) from upperleft
    {None,               sf::Vector2i({17, 0}) },
    {BrownWall1,         sf::Vector2i({11, 13})},
    {WoodWall1,          sf::Vector2i({11, 13})},
    {StoneWall1,         sf::Vector2i({11, 13})},
    {DoorClosed1,        sf::Vector2i({11, 2}) },
    {Stone1,             sf::Vector2i({14, 2}) },
    {Stone2,             sf::Vector2i({14, 2}) },
    {Carpet1,            sf::Vector2i({14, 2}) },
    {Carpet2,            sf::Vector2i({14, 2}) },
    {Carpet3,            sf::Vector2i({14, 2}) },
    {Carpet4,            sf::Vector2i({14, 2}) },
    {Wood1,              sf::Vector2i({14, 2}) },
    {PlayerMaleStanding, sf::Vector2i({0, 4})  },
    {Grass1,             sf::Vector2i({12, 2}) },
    {Grass2,             sf::Vector2i({14, 2}) },
    {DimGrass1,          sf::Vector2i({12, 2}) },
    {DimGrass2,          sf::Vector2i({14, 2}) },
    {Flower1,            sf::Vector2i({10, 2}) },
    {Flower2,            sf::Vector2i({10, 2}) },
    {Mushroom1,          sf::Vector2i({12, 2}) },
    {Mushroom2,          sf::Vector2i({14, 2}) },
    {Tree1,              sf::Vector2i({11, 2}) },
    {Tree2,              sf::Vector2i({11, 2}) },
};

static std::vector<sf::Sprite> SPRITE_REGISTRY(NUM_SPRITES, sf::Sprite(SPRITE_SHEET));

int initSpriteSheet(bool is_ascii);
void generateSpritesVector(bool is_ascii);
sf::Sprite* getSpriteTile(SpriteTiles tile);
