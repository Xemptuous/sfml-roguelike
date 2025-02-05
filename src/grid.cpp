#include "grid.hpp"

#include <random>

extern const int CONSOLE_WIDTH, CONSOLE_HEIGHT;
extern const int MAP_WIDTH  = 300;
extern const int MAP_HEIGHT = 300;
const int NUM_BUILDINGS     = 300;
extern sf::Vector2f SIZE_FACTOR, SCALE_FACTOR;

Tile::Tile() : sprite(*getSpriteTile(None)), bg_sprite(*getSpriteTile(BrownWall1)) {
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
      fg(COLOR_ARRAY[rl::Color::White]),
      bg(COLOR_ARRAY[rl::Color::Black]),
      sprite(*getSpriteTile(stype)),
      bg_sprite(*getSpriteTile(BrownWall1)) {};

Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg)
    : position(pos),
      tile_type(ttype),
      sprite_type(stype),
      fg(COLOR_ARRAY[fg]),
      bg(COLOR_ARRAY[rl::Color::Black]),
      sprite(*getSpriteTile(stype)),
      bg_sprite(*getSpriteTile(BrownWall1)) {};

Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg, rl::Color bg)
    : position(pos),
      tile_type(ttype),
      sprite_type(stype),
      fg(COLOR_ARRAY[fg]),
      bg(COLOR_ARRAY[bg]),
      sprite(*getSpriteTile(stype)),
      bg_sprite(*getSpriteTile(BrownWall1)) {};

void Tile::resetSprite() {
    this->sprite    = *getSpriteTile(this->sprite_type);
    this->bg_sprite = *getSpriteTile(BrownWall1);
}

bool Building::intersects(Building& r) {
    const int b = 2; // buffer between buildings (+1)
    return x1 < r.x2 + b && x2 > r.x1 - b && y1 < r.y2 + b && y2 > r.y1 - b;
}

Grid::Grid() : tiles(std::vector<Tile>{}) {
    tiles.reserve(MAP_WIDTH * MAP_HEIGHT);
}

bool Grid::isWalkable(int x, int y) {
    return (
        x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT && tiles[xy_idx(x, y)].tile_type != Wall
    );
}

void Grid::reloadSprites() {
    for (Tile& tile : this->tiles)
        tile.resetSprite();
}

// Systems
void MapGeneratorSystem(Grid& grid) {
    int n = MAP_HEIGHT * MAP_WIDTH;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> picker(0, 80);

    for (int i = 0; i < n; i++) {
        sf::Vector2i pos = idx_xy(i);
        switch (picker(rng)) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:  grid.tiles.push_back(Tile(pos, Floor, DimGrass1, rl::Color::DarkGreen)); break;
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17: grid.tiles.push_back(Tile(pos, Floor, DimGrass2, rl::Color::DarkGreen)); break;
            case 18: grid.tiles.push_back(Tile(pos, Floor, Grass1, rl::Color::Green)); break;
            case 19: grid.tiles.push_back(Tile(pos, Floor, Grass2, rl::Color::Green)); break;
            case 20: grid.tiles.push_back(Tile(pos, Floor, Flower1, rl::Color::Red)); break;
            case 21: grid.tiles.push_back(Tile(pos, Floor, Flower2, rl::Color::Red)); break;
            case 22: grid.tiles.push_back(Tile(pos, Floor, Mushroom1, rl::Color::Red)); break;
            case 23: grid.tiles.push_back(Tile(pos, Floor, Mushroom2, rl::Color::Gray)); break;
            case 24: grid.tiles.push_back(Tile(pos, Wall, Tree1, rl::Color::SaddleBrown)); break;
            case 25: grid.tiles.push_back(Tile(pos, Wall, Tree2, rl::Color::SaddleBrown)); break;
            default: grid.tiles.push_back(Tile(pos, Floor, None));
        }
    }
}

void BuildingGeneratorSystem(Grid& grid) {
    using namespace rl;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> floor_gen(0, 4);
    std::uniform_int_distribution<int> wall_gen(0, 2);
    std::uniform_int_distribution<int> room_size(7, 12);
    std::uniform_int_distribution<int> room_x(0, MAP_WIDTH - 7);
    std::uniform_int_distribution<int> room_y(0, MAP_HEIGHT - 12);
    std::uniform_int_distribution<int> xy(0, 3);
    std::uniform_int_distribution<int> edgeDist(0, 3);

    std::vector<Building> buildings;
    buildings.reserve(NUM_BUILDINGS);

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
            int floor_type = floor_gen(rng);
            int wall_type  = wall_gen(rng);
            // build structure
            for (int x = b.x1; x <= b.x2; x++) {
                for (int y = b.y1; y <= b.y2; y++) {
                    // if OOB continue
                    if (b.x2 >= MAP_WIDTH || b.y2 >= MAP_HEIGHT) {
                        continue;
                    }
                    // if outer edge, make walls
                    if (x == b.x1 || x == b.x2 || y == b.y1 || y == b.y2) {
                        switch (wall_type) {
                            case 0:
                                grid.tiles[xy_idx(x, y)] =
                                    Tile({x, y}, Wall, BrownWall1, Color::SaddleBrown);
                                break;
                            case 1:
                                grid.tiles[xy_idx(x, y)] =
                                    Tile({x, y}, Wall, WoodWall1, Color::SandyBrown);
                                break;
                            default:
                                grid.tiles[xy_idx(x, y)] =
                                    Tile({x, y}, Wall, StoneWall1, Color::Silver);
                                break;
                        }
                        continue;
                    }
                    switch (floor_type) {
                        case 0:
                            grid.tiles[xy_idx(x, y)] = Tile({x, y}, Floor, Carpet1, Color::Gray);
                            break;
                        case 1:
                            grid.tiles[xy_idx(x, y)] =
                                Tile({x, y}, Floor, Carpet2, Color::SaddleBrown);
                            break;
                        case 2:
                            grid.tiles[xy_idx(x, y)] =
                                Tile({x, y}, Floor, Carpet3, Color::SaddleBrown);
                            break;
                        case 3:
                            grid.tiles[xy_idx(x, y)] =
                                Tile({x, y}, Floor, Carpet4, Color::SaddleBrown);
                            break;
                        default:
                            grid.tiles[xy_idx(x, y)] = Tile({x, y}, Floor, Stone1, Color::Gray);
                            break;
                    }
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
                    grid.tiles[xy_idx(door_x, door_y)] = Tile(
                        {door_x, door_y}, TileType::Floor, SpriteTiles::DoorClosed1, rl::Color::Tan
                    );
                }
            }
        }
    }
}

int xy_idx(int x, int y) {
    return y * MAP_WIDTH + x;
}

sf::Vector2i idx_xy(int idx) {
    return sf::Vector2i{idx % MAP_WIDTH, idx / MAP_WIDTH};
}
