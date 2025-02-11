#include "grid.hpp"

#include <random>

extern const int CONSOLE_WIDTH, CONSOLE_HEIGHT;
extern const int MAP_WIDTH;
extern const int MAP_HEIGHT;
const int NUM_BUILDINGS = 300;

Tile::Tile() : sprite(std::make_shared<sf::Sprite>(*getSpriteTile(None))) {
    this->tile_type   = TileType::Floor;
    this->sprite_type = SpriteTiles::None;
    this->position    = {0, 0};
    this->fg          = sf::Color::White;
    this->bg          = sf::Color::Black;
    this->sprite->setColor(this->fg);
}
Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype) :
    position(pos),
    tile_type(ttype),
    sprite_type(stype),
    fg(COLOR_ARRAY[rl::Color::White]),
    bg(COLOR_ARRAY[rl::Color::Black]),
    sprite(std::make_shared<sf::Sprite>(*getSpriteTile(stype))) {};

Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg) :
    position(pos),
    tile_type(ttype),
    sprite_type(stype),
    fg(COLOR_ARRAY[fg]),
    bg(COLOR_ARRAY[rl::Color::Black]),
    sprite(std::make_shared<sf::Sprite>(*getSpriteTile(stype))) {};

Tile::Tile(sf::Vector2i pos, TileType ttype, SpriteTiles stype, rl::Color fg, rl::Color bg) :
    position(pos),
    tile_type(ttype),
    sprite_type(stype),
    fg(COLOR_ARRAY[fg]),
    bg(COLOR_ARRAY[bg]),
    sprite(std::make_shared<sf::Sprite>(*getSpriteTile(stype))) {};

void Tile::resetSprite() {
    this->sprite = std::make_shared<sf::Sprite>(*getSpriteTile(this->sprite_type));
}

bool Building::intersects(Building& r) {
    const int b = 2; // buffer between buildings (+1)
    return x1 < r.x2 + b && x2 > r.x1 - b && y1 < r.y2 + b && y2 > r.y1 - b;
}

Grid::Grid() : tiles(std::vector<std::shared_ptr<Tile>>{}) {
    tiles.reserve(MAP_WIDTH * MAP_HEIGHT);
}

bool Grid::isWalkable(int x, int y) {
    return (
        x >= 0 && y >= 0 && x < MAP_WIDTH && y < MAP_HEIGHT
        && tiles[xy_idx(x, y)]->tile_type != Wall
    );
}

void Grid::reloadSprites() {
    for (std::shared_ptr<Tile> tile : this->tiles)
        tile->resetSprite();
}

// Systems
void MapGeneratorSystem(Grid& grid) {
    int n = MAP_HEIGHT * MAP_WIDTH;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> picker(0, 100);
    using namespace rl;

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
            case 8:
            case 9:
            case 10:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, DimGrass1, DarkGreen))
                );
                break;
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, DimGrass2, DarkGreen))
                );
                break;
            case 22:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, Grass1, Green)));
                break;
            case 23:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, Grass2, Green)));
                break;
            case 24:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, Flower1, DarkRed)));
                break;
            // case 21: grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, Flower2,
            // Red))); break;
            case 26:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, Mushroom1, DarkRed)));
                break;
            case 27:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, Mushroom2, Gray)));
                break;
            case 28:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Wall, Tree1, SaddleBrown)));
                break;
            case 29:
                grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Wall, Tree2, SaddleBrown)));
                break;
            default: grid.tiles.push_back(std::make_shared<Tile>(Tile(pos, Floor, None)));
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
                                grid.tiles[xy_idx(x, y)] = std::make_shared<Tile>(
                                    Tile({x, y}, Wall, BrownWall1, rl::Color::SaddleBrown)
                                );
                                break;
                            case 1:
                                grid.tiles[xy_idx(x, y)] = std::make_shared<Tile>(
                                    Tile({x, y}, Wall, WoodWall1, rl::Color::SandyBrown)
                                );
                                break;
                            default:
                                grid.tiles[xy_idx(x, y)] = std::make_shared<Tile>(
                                    Tile({x, y}, Wall, StoneWall1, rl::Color::Silver)
                                );
                                break;
                        }
                        continue;
                    }
                    switch (floor_type) {
                        case 0:
                            grid.tiles[xy_idx(x, y)] =
                                std::make_shared<Tile>(Tile({x, y}, Floor, Carpet1, rl::Color::Gray)
                                );
                            break;
                        case 1:
                            grid.tiles[xy_idx(x, y)] = std::make_shared<Tile>(
                                Tile({x, y}, Floor, Carpet2, rl::Color::SaddleBrown)
                            );
                            break;
                        case 2:
                            grid.tiles[xy_idx(x, y)] = std::make_shared<Tile>(
                                Tile({x, y}, Floor, Carpet3, rl::Color::SaddleBrown)
                            );
                            break;
                        case 3:
                            grid.tiles[xy_idx(x, y)] = std::make_shared<Tile>(
                                Tile({x, y}, Floor, Carpet4, rl::Color::SaddleBrown)
                            );
                            break;
                        default:
                            grid.tiles[xy_idx(x, y)] =
                                std::make_shared<Tile>(Tile({x, y}, Floor, Stone1, rl::Color::Gray)
                                );
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
                    grid.tiles[xy_idx(door_x, door_y)] = std::make_shared<Tile>(Tile(
                        {door_x, door_y}, TileType::Floor, SpriteTiles::DoorClosed1, rl::Color::Tan
                    ));
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
