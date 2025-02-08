#include "sprite.hpp"

#include "engine.hpp"

extern const int SPRITE_WIDTH  = 12;
extern const int SPRITE_HEIGHT = 12;
// extern sf::Font font;

extern Options OPTIONS;

using namespace sf;

int SpritesheetLoadingSystem() {
    auto sprite_type = OPTIONS.is_ascii ? "../include/alloy_curses_12x12.png"
                                        : "../include/urizen_onebit_tileset__v1d1.png";
    ;
    if (!SPRITE_SHEET_IMAGE.loadFromFile(sprite_type)) {
        printf("Could not load sprite sheet!\n");
        return 2;
    }
    // transparency layer
    OPTIONS.is_ascii ? SPRITE_SHEET_IMAGE.createMaskFromColor({255, 0, 255, 255})
                     : SPRITE_SHEET_IMAGE.createMaskFromColor({0, 0, 0, 255});

    if (!SPRITE_SHEET.loadFromImage(SPRITE_SHEET_IMAGE)) {
        printf("Could not load sprite sheet!\n");
        return 2;
    }
    return 0;
}

void SpriteGenerator() {
    auto sprites = OPTIONS.is_ascii ? ASCII_COORDINATES : SPRITE_COORDINATES;

    for (const auto& pair : sprites) {
        SpriteTiles tile_type = pair.first;

        Vector2i sprite_coord = pair.second;

        // +1 to account for initial border
        Vector2i coords = OPTIONS.is_ascii ? Vector2i{
                SPRITE_WIDTH * sprite_coord.x,
                SPRITE_HEIGHT * sprite_coord.y,
            } : Vector2i{
                SPRITE_WIDTH * sprite_coord.x + sprite_coord.x + 1,
                SPRITE_HEIGHT * sprite_coord.y + sprite_coord.y + 1,
            };
        Vector2i size = {
            SPRITE_WIDTH,
            SPRITE_HEIGHT,
        };

        SPRITE_REGISTRY[tile_type] = Sprite(SPRITE_SHEET, {coords, size});
    }
}

Sprite* getSpriteTile(SpriteTiles tile) {
    return &SPRITE_REGISTRY[tile];
}

SpriteTiles strToSpriteTile(std::string s) {
    if (s == "barbarian") return BarbarianUnarmed;
    if (s == "goblin") return GoblinUnarmed;
    if (s == "demon") return DemonUnarmed;
    return None;
};
