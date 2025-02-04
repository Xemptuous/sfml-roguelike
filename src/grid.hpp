#pragma once

#include "color.hpp"
#include "sprite.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

enum TileType {
    Wall,
    Floor,
};

struct Position {
    int x;
    int y;
};

struct Tile {
    sf::Sprite sprite;
    sf::Sprite bg_sprite;
    TileType tile_type;
    SpriteTiles sprite_type;
    sf::Vector2i position;
    sf::Color fg;
    sf::Color bg;

    Tile();
    Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype);
    Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg);
    Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg, rl::Color bg);

    void resetSprite();
};

struct Building {
    int x1, y1, x2, y2;

    bool intersects(Building&);
};

struct Grid {
    std::vector<Tile> tiles;
    std::vector<Building> buildings;
    Grid();

    std::vector<Tile> generateMapTiles();
    void generateBuildings();
};

int xy_idx(int x, int y);
std::pair<int, int> idx_xy(int);
