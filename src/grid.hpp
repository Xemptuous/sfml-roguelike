#pragma once

#include "color.hpp"
#include "sprite.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <vector>

struct Position {
    int x;
    int y;
};
enum TileType {
    Wall,
    Floor,
};

// Components
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
    bool isWalkable() const { return tile_type == TileType::Floor; };
};

struct Building {
    int x1, y1, x2, y2;
    bool intersects(Building&);
};

struct Grid {
    std::vector<Tile> tiles;

    Grid();
    bool isWalkable(int x, int y);
    void updateMap();
    void reloadSprites();
};

int xy_idx(int x, int y);
sf::Vector2i idx_xy(int);

// Systems
void MapGeneratorSystem(Grid& grid);
void BuildingGeneratorSystem(Grid& grid);
