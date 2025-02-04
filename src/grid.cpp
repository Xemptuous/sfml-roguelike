#include "grid.hpp"

#include "sprite.hpp"

#include <random>

extern const int CONSOLE_WIDTH, CONSOLE_HEIGHT;
extern const int MAP_WIDTH  = 300;
extern const int MAP_HEIGHT = 300;
const int NUM_BUILDINGS     = 300;

extern sf::RenderTexture renderTexture;

Grid::Grid() {
    this->tiles = this->generateMapTiles();
    this->generateBuildings();
}

Tile::Tile() : sprite(*getSpriteTile(None)), bg_sprite(*getSpriteTile(Wall1)) {
    this->tile_type   = TileType::Floor;
    this->sprite_type = SpriteTiles::None;
    this->position    = {0, 0};
    this->fg          = sf::Color::White;
    this->bg          = sf::Color::Black;
    this->sprite.setColor(this->fg);
    this->bg_sprite.setColor(this->bg);
}
Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype)
    : position(pos),
      tile_type(ttype),
      sprite_type(stype),
      fg(COLOR_ARRAY[rl::Color::Black]),
      bg(COLOR_ARRAY[rl::Color::Black]),
      sprite(*getSpriteTile(stype)),
      bg_sprite(*getSpriteTile(Wall1)) {};

Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg)
    : position(pos),
      tile_type(ttype),
      sprite_type(stype),
      fg(COLOR_ARRAY[fg]),
      bg(COLOR_ARRAY[rl::Color::Black]),
      sprite(*getSpriteTile(stype)),
      bg_sprite(*getSpriteTile(Wall1)) {};

Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg, rl::Color bg)
    : position(pos),
      tile_type(ttype),
      sprite_type(stype),
      fg(COLOR_ARRAY[fg]),
      bg(COLOR_ARRAY[bg]),
      sprite(*getSpriteTile(stype)),
      bg_sprite(*getSpriteTile(Wall1)) {};

void Tile::resetSprite() {
    this->sprite    = *getSpriteTile(this->sprite_type);
    this->bg_sprite = *getSpriteTile(Wall1);
}

std::vector<Tile> Grid::generateMapTiles() {
    std::vector<Tile> tiles = std::vector<Tile>{};

    int n = MAP_HEIGHT * MAP_WIDTH;
    tiles.reserve(n);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> picker(0, 80);

    for (int i = 0; i < n; i++) {
        auto c = idx_xy(i);
        int x = c.first, y = c.second;
        switch (picker(rng)) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:  tiles.push_back(Tile({x, y}, Floor, DimGrass1, rl::Color::DarkGreen)); break;
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17: tiles.push_back(Tile({x, y}, Floor, DimGrass2, rl::Color::DarkGreen)); break;
            case 18: tiles.push_back(Tile({x, y}, Floor, Grass1, rl::Color::Green)); break;
            case 19: tiles.push_back(Tile({x, y}, Floor, Grass2, rl::Color::Green)); break;
            case 20: tiles.push_back(Tile({x, y}, Floor, Flower1, rl::Color::Red)); break;
            case 21: tiles.push_back(Tile({x, y}, Floor, Flower2, rl::Color::Red)); break;
            case 22: tiles.push_back(Tile({x, y}, Floor, Mushroom1, rl::Color::Red)); break;
            case 23: tiles.push_back(Tile({x, y}, Floor, Mushroom2, rl::Color::Gray)); break;
            case 24: tiles.push_back(Tile({x, y}, Wall, Tree1, rl::Color::SaddleBrown)); break;
            case 25: tiles.push_back(Tile({x, y}, Wall, Tree2, rl::Color::SaddleBrown)); break;
            default: tiles.push_back(Tile({x, y}, Floor, None));
        }
    }
    return tiles;
}

bool Building::intersects(Building& r) {
    const int b = 2; // buffer between buildings (+1)
    return x1 < r.x2 + b && x2 > r.x1 - b && y1 < r.y2 + b && y2 > r.y1 - b;
}

void Grid::generateBuildings() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> room_size(7, 12);
    std::uniform_int_distribution<int> room_x(0, MAP_WIDTH - 7);
    std::uniform_int_distribution<int> room_y(0, MAP_HEIGHT - 12);
    std::uniform_int_distribution<int> xy(0, 3);
    std::uniform_int_distribution<int> edgeDist(0, 3);

    this->buildings.reserve(NUM_BUILDINGS);

    for (int i = 0; i < NUM_BUILDINGS; i++) {
        int width  = room_size(rng);
        int height = room_size(rng);
        int x1 = room_x(rng), x2 = width + x1;
        int y1 = room_y(rng), y2 = height + y1;

        Building b = Building{x1, y1, x2, y2};

        bool to_build = true;
        for (Building& other : buildings) {
            to_build = (!b.intersects(other));
            if (!to_build) break;
        }

        if (to_build) {
            buildings.push_back(b);
            // build structure
            for (int x = b.x1; x <= b.x2; x++) {
                for (int y = b.y1; y <= b.y2; y++) {
                    // if OOB continue
                    if (b.x2 >= MAP_WIDTH || b.y2 >= MAP_HEIGHT) {
                        continue;
                    }
                    // if outer edge, make walls
                    if (x == b.x1 || x == b.x2 || y == b.y1 || y == b.y2) {
                        tiles[xy_idx(x, y)] =
                            Tile({x, y}, TileType::Wall, SpriteTiles::Wall1, rl::Color::SandyBrown);
                        continue;
                    }
                    tiles[xy_idx(x, y)] =
                        Tile({x, y}, TileType::Floor, SpriteTiles::Stone1, rl::Color::Gray);
                }
            }
            // add a door randomly along an edge of the building
            // Pick a random edge (0=top, 1=bottom, 2=left, 3=right)
            int edge = edgeDist(rng);
            int door_x, door_y;

            if (width > 2 && height > 2) {
                switch (edge) {
                    case 0: // Top edge
                        // Avoid corners
                        door_x = x1 + (rng() % (width - 2)) + 1;
                        door_y = y1;
                        break;
                    case 1: // Bottom edge
                        door_x = x1 + (rng() % (width - 2)) + 1;
                        door_y = y2;
                        break;
                    case 2: // Left edge
                        door_x = x1;
                        door_y = y1 + (rng() % (height - 2)) + 1;
                        break;
                    case 3: // Right edge
                        door_x = x2;
                        door_y = y1 + (rng() % (height - 2)) + 1;
                        break;
                }

                // Replace the chosen wall tile with a door
                if (door_x >= 0 && door_x < MAP_WIDTH && door_y >= 0 && door_y < MAP_HEIGHT) {
                    tiles[xy_idx(door_x, door_y)] =
                        Tile({door_x, door_y}, TileType::Floor, SpriteTiles::Door1, rl::Color::Tan);
                }
            }

            // // Add rooms to buildings
            // edge = edgeDist(rng);
            //
            // int roomWidth  = room_size(rng) / 2; // Smaller than main building
            // int roomHeight = room_size(rng) / 2;
            // int rx1, ry1, rx2, ry2;
            //
            // switch (edge) {
            //     case 0: // Top edge
            //         rx1 = x1 + (width - roomWidth) / 2;
            //         ry1 = y1 - roomHeight;
            //         rx2 = rx1 + roomWidth;
            //         ry2 = y1;
            //         break;
            //     case 1: // Bottom edge
            //         rx1 = x1 + (width - roomWidth) / 2;
            //         ry1 = y2;
            //         rx2 = rx1 + roomWidth;
            //         ry2 = ry1 + roomHeight;
            //         break;
            //     case 2: // Left edge
            //         rx1 = x1 - roomWidth;
            //         ry1 = y1 + (height - roomHeight) / 2;
            //         rx2 = x1;
            //         ry2 = ry1 + roomHeight;
            //         break;
            //     case 3: // Right edge
            //         rx1 = x2;
            //         ry1 = y1 + (height - roomHeight) / 2;
            //         rx2 = rx1 + roomWidth;
            //         ry2 = ry1 + roomHeight;
            //         break;
            // }
            //
            // if (rx1 >= 0 && rx2 < MAP_WIDTH && ry1 >= 0 && ry2 < MAP_HEIGHT) {
            //     // Ensure the room does not overlap another building
            //     Building room       = Building{rx1, ry1, rx2, ry2};
            //     bool can_place_room = true;
            //     for (Building& other : buildings) {
            //         if (room.intersects(other)) {
            //             can_place_room = false;
            //             break;
            //         }
            //     }
            //
            //     if (can_place_room) {
            //         buildings.push_back(room);
            //
            //         // Build the room
            //         for (int x = room.x1; x <= room.x2; x++) {
            //             for (int y = room.y1; y <= room.y2; y++) {
            //                 // If outer edge, make walls
            //                 if (x == room.x1 || x == room.x2 || y == room.y1 || y == room.y2) {
            //                     tiles[xy_idx(x, y)] =
            //                         Tile({x, y}, TileType::Wall, SpriteTiles::Wall1);
            //                     continue;
            //                 }
            //                 tiles[xy_idx(x, y)] =
            //                     Tile({x, y}, TileType::Floor, SpriteTiles::Stone1);
            //             }
            //         }
            //         // Add a door between the main building and the room
            //         int door_x, door_y;
            //         switch (edge) {
            //             case 0: // Top edge
            //                 door_x = rx1 + roomWidth / 2;
            //                 door_y = y1;
            //                 break;
            //             case 1: // Bottom edge
            //                 door_x = rx1 + roomWidth / 2;
            //                 door_y = y2;
            //                 break;
            //             case 2: // Left edge
            //                 door_x = x1;
            //                 door_y = ry1 + roomHeight / 2;
            //                 break;
            //             case 3: // Right edge
            //                 door_x = x2;
            //                 door_y = ry1 + roomHeight / 2;
            //                 break;
            //         }
            //         tiles[xy_idx(door_x, door_y)] =
            //             Tile({door_x, door_y}, TileType::Floor, SpriteTiles::Door1);
            //     }
            // }
        }
    }
}

int xy_idx(int x, int y) {
    return y * MAP_WIDTH + x;
}

std::pair<int, int> idx_xy(int idx) {
    return std::make_pair(idx % MAP_WIDTH, idx / MAP_WIDTH);
}
