#include "engine.hpp"

#include "grid.hpp"
#include "player.hpp"
#include "sprite.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

extern const int RENDER_WIDTH, RENDER_HEIGHT;
extern const int CONSOLE_WIDTH, CONSOLE_HEIGHT;
extern const int SPRITE_WIDTH, SPRITE_HEIGHT;
extern const int MAP_WIDTH, MAP_HEIGHT;

extern sf::RenderTexture renderTexture;
extern sf::RenderWindow window;
extern sf::View view;

Engine::Engine(Options options) : options(options) {
    this->player = new Player(SpriteTiles::PlayerMaleStanding);
    this->grid   = new Grid();
    this->update();
}

Engine::~Engine() {
    delete this->player;
    delete this->grid;
}

void Engine::update() {
    this->updateScreenSize();
    this->updateViewPosition();
    this->updateViewDimensions();
    this->updateMap();
}

void Engine::handleKey() {
    using namespace sf::Keyboard;
    int idx;
    if (isKeyPressed(Key::Left)) tryMovePlayer(-1, 0);
    if (isKeyPressed(Key::Right)) tryMovePlayer(1, 0);
    if (isKeyPressed(Key::Up)) tryMovePlayer(0, -1);
    if (isKeyPressed(Key::Down)) tryMovePlayer(0, 1);

    // FIXME: doesn't work :(
    if (isKeyPressed(Key::K)) tryMoveView(0, -1);
    if (isKeyPressed(Key::H)) tryMoveView(-1, 0);
    if (isKeyPressed(Key::J)) tryMoveView(0, 1);
    if (isKeyPressed(Key::L)) tryMoveView(1, 0);
}

void Engine::tryMoveView(int dx, int dy) {
    int nx1 = view_rect.x1 + dx;
    int nx2 = view_rect.x2 + dx;
    int ny1 = view_rect.y1 + dy;
    int ny2 = view_rect.y2 + dy;

    if (nx1 < 0 || ny1 < 0 || nx2 > MAP_WIDTH || ny2 > MAP_HEIGHT) {
        return;
    }
    view.move({
        dx * SPRITE_WIDTH * scale_factor.x,
        dy * SPRITE_HEIGHT * scale_factor.y,
    });

    // Recalculate view rect
    updateViewDimensions();
}

void Engine::tryMovePlayer(int dx, int dy) {
    int dest_x = player->position.x + dx;
    int dest_y = player->position.y + dy;

    // if OOB
    if (dest_x < 0 || dest_y < 0 || dest_x >= MAP_WIDTH || dest_y >= MAP_HEIGHT) return;

    int idx = xy_idx(dest_x, dest_y);

    if (this->grid->tiles[idx].tile_type == TileType::Wall) return;

    player->position.x = dest_x;
    player->position.y = dest_y;

    // printf("Player: (%d, %d)\n", player->position.x, player->position.y);

    bool can_move_view_x = true;
    bool can_move_view_y = true;

    int view_cw = (view_rect.x2 - view_rect.x1) / 2;
    int view_ch = (view_rect.y2 - view_rect.y1) / 2;

    bool view_at_left   = view_rect.x1 <= 0;
    bool view_at_right  = view_rect.x2 >= MAP_WIDTH;
    bool view_at_top    = view_rect.y1 <= 0;
    bool view_at_bottom = view_rect.y2 >= MAP_HEIGHT;

    bool player_left_of_center  = player->position.x <= view_rect.x2 / 2;
    bool player_right_of_center = player->position.x >= MAP_WIDTH - view_cw;
    bool player_above_center    = player->position.y <= view_rect.y2 / 2;
    bool player_below_center    = player->position.y >= MAP_HEIGHT - view_ch;

    if (dx < 0 && (view_at_left || (view_at_right && player_right_of_center)))
        can_move_view_x = false;
    else if (dx > 0 && (view_at_right || (view_at_left && player_left_of_center)))
        can_move_view_x = false;
    else if (dy < 0 && (view_at_top || (view_at_bottom && player_below_center)))
        can_move_view_y = false;
    else if (dy > 0 && (view_at_bottom || (view_at_top && player_above_center)))
        can_move_view_y = false;

    if (can_move_view_x) tryMoveView(dx, 0);
    if (can_move_view_y) tryMoveView(0, dy);
}

void Engine::drawEntities() {
    /*
    Ww / Cw * Player.x = x position
    Wh / Ch * Player.y = y position
    */
    printf("DRAW\n");
    Vector2f pos{
        std::round(size_factor.x * player->position.x),
        std::round(size_factor.y * player->position.y),
    };
    player->sprite->setPosition(pos);
    player->sprite->setScale(scale_factor);
    if (options.is_ascii) {
        Sprite* bg = getSpriteTile(SpriteTiles::Wall1);
        bg->setPosition(pos);
        bg->setColor(sf::Color::Black);
        renderTexture.draw(*bg);
    }
    renderTexture.draw(*player->sprite);
}

void Engine::updateMap() {
    for (Tile& tile : this->grid->tiles) {
        Vector2f pos{
            std::round(size_factor.x * tile.position.x),
            std::round(size_factor.y * tile.position.y),
        };
        tile.sprite.setPosition(pos);
        tile.sprite.setScale(scale_factor);
        if (options.is_ascii) {
            tile.bg_sprite.setPosition(pos);
            tile.bg_sprite.setColor(tile.bg);
            tile.sprite.setColor(tile.fg);
        }
    }
}

void Engine::drawMap() {
    // draw tiles within the current view
    for (int x = view_rect.x1; x < view_rect.x2; x++) {
        for (int y = view_rect.y1; y < view_rect.y2; y++) {
            int idx   = xy_idx(x, y);
            Tile tile = grid->tiles[idx];
            if (options.is_ascii) {
                renderTexture.draw(tile.bg_sprite);
            }
            renderTexture.draw(tile.sprite);
        }
    }
}

void Engine::updateScreenSize() {
    /*
    Window: 800x600 (Ww x Wh)
    Console: 80x50 (Cw x Ch)
    Tileset: 12x12 (Tw x Th)

    Positioning:
    Ww / Cw = width size factor
    Wh / Ch = height size factor

    Scaling:
    (Ww / Cw) / Tw = width scale factor
    (Wh / Ch) / Th = height scale factor
    */
    size_factor = Vector2f{
        // (float)screen_size.x / CONSOLE_WIDTH,
        // (float)screen_size.y / CONSOLE_HEIGHT,
        (float)RENDER_WIDTH / CONSOLE_WIDTH,
        (float)RENDER_HEIGHT / CONSOLE_HEIGHT,
    };

    scale_factor = Vector2f{
        size_factor.x / SPRITE_WIDTH,
        size_factor.y / SPRITE_HEIGHT,
    };

    // printf(
    //     "RENDER WxH (%d, %d)  Screen WxH: (%d, %d)\n", RENDER_WIDTH, RENDER_HEIGHT,
    //     screen_size.x, screen_size.y
    // );
}
void Engine::updateViewPosition() {
    view.setCenter({
        player->position.x * size_factor.x,
        player->position.y * size_factor.y - CONSOLE_HEIGHT,
    });
    view.setSize(Vector2f{
        CONSOLE_WIDTH * size_factor.x,
        CONSOLE_HEIGHT * size_factor.y,
    });
}

// Recalculate view rect
void Engine::updateViewDimensions() {
    Vector2f size   = view.getSize();
    Vector2f center = view.getCenter();

    screen_size = window.getSize();
    // printf(
    //     "SCREEN WxH (%d, %d)  screen WxH: (%d, %d)\n", RENDER_WIDTH, RENDER_HEIGHT,
    //     screen_size.x, screen_size.y
    // );
    // printf("View SIZE: (%f, %f) CENTER (%f, %f)\n", size.x, size.y, center.x, center.y);
    // printf(
    //     "Factor Size: (%f, %f)  Scale: (%f, %f)\n", size_factor.x, size_factor.y, scale_factor.x,
    //     scale_factor.y
    // );

    int cx = center.x / size_factor.x;
    int cy = center.y / size_factor.y;
    int sx = size.x / size_factor.x;
    int sy = size.y / size_factor.y;

    int x1 = std::max(0, cx - sx / 2);
    int y1 = std::max(0, cy - sy / 2);

    int x2 = std::min(MAP_WIDTH, x1 + sx);
    int y2 = std::min(MAP_HEIGHT, y1 + sy);

    ViewRect vr = {x1, y1, x2, y2};

    this->view_rect = vr;
    // printf("View Rect: x1y1(%d, %d) x2y2(%d, %d)\n\n", vr.x1, vr.y1, vr.x2, vr.y2);
}

void Engine::swapTilesets() {
    options.is_ascii = !options.is_ascii;
    initSpriteSheet(options.is_ascii);
    SPRITE_REGISTRY.clear();
    generateSpritesVector(options.is_ascii);
    for (Tile& tile : grid->tiles)
        tile.resetSprite();
    updateMap();
}
