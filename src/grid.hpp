#pragma once

#include "color.hpp"
#include "sprite.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

constexpr int MAP_WIDTH  = 100;
constexpr int MAP_HEIGHT = 100;

enum TileType {
    Wall,
    Floor,
};

// Components
struct Tile {
    std::shared_ptr<sf::Sprite> sprite;
    TileType tile_type;
    SpriteTiles sprite_type;
    sf::Vector2i position;
    sf::Color fg;
    sf::Color bg;
    // bool seen;

    Tile();
    Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype);
    Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg);
    Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg, rl::Color bg);

    void resetSprite();
    bool isWalkable() const { return tile_type == TileType::Floor; };
};

struct Building {
    int x1, y1, x2, y2;
    bool intersects(Building&);
};

struct Grid {
    std::vector<std::shared_ptr<Tile>> tiles;

    Grid();

    bool isWalkable(int x, int y);
    bool isWalkable(sf::Vector2i);
    void updateMap();
    void reloadSprites();
};

int xy_idx(int x, int y);
sf::Vector2i idx_xy(int);

// Systems
void MapGeneratorSystem(Grid& grid);
void BuildingGeneratorSystem(Grid& grid);
