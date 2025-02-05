#include "engine.hpp"

#include "grid.hpp"
#include "sprite.hpp"

#include <SFML/Window/Window.hpp>
#include <algorithm>
#include <cmath>
#include <unordered_set>

extern int MAP_WIDTH, MAP_HEIGHT;
extern int SPRITE_WIDTH, SPRITE_HEIGHT;
extern int RENDER_WIDTH, RENDER_HEIGHT;
extern int CONSOLE_WIDTH, CONSOLE_HEIGHT;

Options OPTIONS;
Vector2f SIZE_FACTOR{};
Vector2f SCALE_FACTOR{};

// Component Storage
std::unordered_map<Entity, Position> positions{};
std::unordered_map<Entity, Movement> movements{};
std::unordered_map<Entity, Renderable> renderables{};

// Systems
void DrawSystem(RenderWindow& window, RenderTexture& renderTexture, Camera& camera, Grid& grid) {
    // printf("DrawSystem\n");
    window.clear();
    renderTexture.clear();
    renderTexture.setView(camera.view);

    RenderSystem(renderTexture, camera, grid);

    renderTexture.display();
    const sf::Texture& texture = renderTexture.getTexture();
    sf::Sprite sprite(texture);

    window.draw(sprite);
    window.display();
}

void RenderSystem(RenderTexture& renderTexture, Camera& camera, Grid& grid) {
    // printf("RenderSystem\n");
    // draw the map
    for (int x = camera.x1; x < camera.x2; x++) {
        for (int y = camera.y1; y < camera.y2; y++) {
            int idx   = xy_idx(x, y);
            Tile tile = grid.tiles[idx];
            if (OPTIONS.is_ascii) {
                renderTexture.draw(tile.bg_sprite);
            }
            renderTexture.draw(tile.sprite);
        }
    }

    // draw entities
    std::vector<std::pair<Entity, Renderable>> renderQueue;
    for (auto& [entity, renderable] : renderables) {
        renderQueue.push_back({entity, renderable});
    }
    std::sort(renderQueue.begin(), renderQueue.end(), [](auto& a, auto& b) {
        return a.second.zIndex < b.second.zIndex;
    });

    for (auto& [entity, renderable] : renderQueue) {
        if (positions.count(entity)) {
            const Position& pos = positions[entity];

            // draw
            // renderTexture.draw(renderable.sprite);

            Vector2f posv{
                std::round(SIZE_FACTOR.x * positions[entity].x),
                std::round(SIZE_FACTOR.y * positions[entity].y),
            };
            renderable.sprite.setPosition(posv);
            renderable.sprite.setScale(SCALE_FACTOR);
            if (OPTIONS.is_ascii) {
                Sprite* bg = getSpriteTile(SpriteTiles::WoodWall1);
                bg->setPosition(posv);
                bg->setColor(sf::Color::Black);
                renderTexture.draw(*bg);
            }
            renderTexture.draw(renderable.sprite);
        }
    }
}

void InputSystem(Entity& player) {
    // printf("InputSystem\n");
    using namespace sf::Keyboard;
    int dx = 0, dy = 0;
    if (isKeyPressed(Key::Left)) dx = -1;
    if (isKeyPressed(Key::Right)) dx = 1;
    if (isKeyPressed(Key::Up)) dy = -1;
    if (isKeyPressed(Key::Down)) dy = 1;

    if (dx != 0 || dy != 0) {
        movements[player] = {dx, dy};
    }

    // if (isKeyPressed(Key::K)) tryMoveView(1, -1);
    // if (isKeyPressed(Key::H)) tryMoveView(-1, 0);
    // if (isKeyPressed(Key::J)) tryMoveView(0, 1);
    // if (isKeyPressed(Key::L)) tryMoveView(1, 0);
}

void CollisionSystem(Grid& grid) {
    for (auto& [entity, pos] : positions) {
        if (movements.count(entity)) {
            int dest_x = pos.x;
            int dest_y = pos.y;

            if (!grid.isWalkable(dest_x, dest_y)) {
                // undo movement if colliding
                pos.x -= movements[entity].dx;
                pos.y -= movements[entity].dy;

                movements[entity].dx = 0;
                movements[entity].dy = 0;
            }
        }
    }
}

void MovementSystem(std::unordered_set<Entity>& entities) {
    for (Entity entity : entities) {
        if (positions.count(entity) && movements.count(entity)) {
            positions[entity].x += movements[entity].dx;
            positions[entity].y += movements[entity].dy;
        }
    }
}

void CameraSystem(Entity& player, Camera& camera) {
    if (positions.count(player)) {
        Position& playerPos = positions[player];
        Movement& playerMov = movements[player];

        bool can_move_view_x = true;
        bool can_move_view_y = true;

        int view_cw = (camera.x2 - camera.x1) / 2;
        int view_ch = (camera.y2 - camera.y1) / 2;

        bool view_at_left   = camera.x1 <= 0;
        bool view_at_right  = camera.x2 >= MAP_WIDTH;
        bool view_at_top    = camera.y1 <= 0;
        bool view_at_bottom = camera.y2 >= MAP_HEIGHT;

        bool player_left_of_center  = playerPos.x <= camera.x2 / 2;
        bool player_right_of_center = playerPos.x >= MAP_WIDTH - view_cw;
        bool player_above_center    = playerPos.y <= camera.y2 / 2;
        bool player_below_center    = playerPos.y >= MAP_HEIGHT - view_ch;

        if (playerMov.dx < 0 && (view_at_left || (view_at_right && player_right_of_center)))
            can_move_view_x = false;
        else if (playerMov.dx > 0 && (view_at_right || (view_at_left && player_left_of_center)))
            can_move_view_x = false;
        else if (playerMov.dy < 0 && (view_at_top || (view_at_bottom && player_below_center)))
            can_move_view_y = false;
        else if (playerMov.dy > 0 && (view_at_bottom || (view_at_top && player_above_center)))
            can_move_view_y = false;

        if (can_move_view_x) {
            int nx1 = camera.x1 + playerMov.dx;
            int nx2 = camera.x2 + playerMov.dx;

            if (nx1 < 0 || nx2 > MAP_WIDTH) {
                return;
            }
            camera.view.move({
                playerMov.dx * SPRITE_WIDTH * SCALE_FACTOR.x,
                0,
            });
            ResizeCameraSystem(camera);
        };

        if (can_move_view_y) {
            int ny1 = camera.y1 + playerMov.dy;
            int ny2 = camera.y2 + playerMov.dy;

            if (ny1 < 0 || ny2 > MAP_HEIGHT) {
                return;
            }
            camera.view.move({
                0,
                playerMov.dy * SPRITE_HEIGHT * SCALE_FACTOR.y,
            });
            ResizeCameraSystem(camera);
        };

        // if (can_move_view_x || can_move_view_y) {
        //     int nx1 = camera.x1 + playerMov.dx;
        //     int nx2 = camera.x2 + playerMov.dx;
        //     int ny1 = camera.y1 + playerMov.dy;
        //     int ny2 = camera.y2 + playerMov.dy;
        //
        //     if (nx1 < 0 || ny1 < 0 || nx2 > MAP_WIDTH || ny2 > MAP_HEIGHT) {
        //         return;
        //     }
        //     camera.view.move({
        //         playerMov.dx * SPRITE_WIDTH * SCALE_FACTOR.x,
        //         playerMov.dy * SPRITE_HEIGHT * SCALE_FACTOR.y,
        //     });
        //     ResizeCameraSystem(camera);
        // };
    }
}

void ResizeSystem(Entity& player, Camera& camera, Grid& grid) {
    // update scale and size factors
    SIZE_FACTOR = Vector2f{
        (float)RENDER_WIDTH / CONSOLE_WIDTH,
        (float)RENDER_HEIGHT / CONSOLE_HEIGHT,
    };
    SCALE_FACTOR = Vector2f{
        SIZE_FACTOR.x / SPRITE_WIDTH,
        SIZE_FACTOR.y / SPRITE_HEIGHT,
    };

    // update camera position
    camera.view.setCenter({
        positions[player].x * SIZE_FACTOR.x,
        positions[player].y * SIZE_FACTOR.y - CONSOLE_HEIGHT,
    });
    camera.view.setSize(Vector2f{
        CONSOLE_WIDTH * SIZE_FACTOR.x,
        CONSOLE_HEIGHT * SIZE_FACTOR.y,
    });
    // update camera dimensions
    ResizeCameraSystem(camera);

    // resize entities
    for (auto& [entity, renderable] : renderables) {
        Vector2f pos{
            std::round(SIZE_FACTOR.x * positions[entity].x),
            std::round(SIZE_FACTOR.y * positions[entity].y),
        };
        renderable.sprite.setPosition(pos);
        renderable.sprite.setScale(SCALE_FACTOR);
    }

    // resize map tiles
    for (Tile& tile : grid.tiles) {
        Vector2f pos{
            std::round(SIZE_FACTOR.x * tile.position.x),
            std::round(SIZE_FACTOR.y * tile.position.y),
        };
        tile.sprite.setPosition(pos);
        tile.sprite.setScale(SCALE_FACTOR);
        if (OPTIONS.is_ascii) {
            tile.bg_sprite.setPosition(pos);
            tile.bg_sprite.setScale(SCALE_FACTOR);
            tile.bg_sprite.setColor(tile.bg);
            tile.sprite.setColor(tile.fg);
        }
    }
}

void ResizeCameraSystem(Camera& camera) {
    Vector2f size   = camera.view.getSize();
    Vector2f center = camera.view.getCenter();

    int cx = center.x / SIZE_FACTOR.x;
    int cy = center.y / SIZE_FACTOR.y;
    int sx = size.x / SIZE_FACTOR.x;
    int sy = size.y / SIZE_FACTOR.y;

    int x1 = std::max(0, cx - sx / 2);
    int y1 = std::max(0, cy - sy / 2);

    int x2 = std::min(MAP_WIDTH, x1 + sx);
    int y2 = std::min(MAP_HEIGHT, y1 + sy);

    camera.x1 = x1;
    camera.x2 = x2;
    camera.y1 = y1;
    camera.y2 = y2;
};

void SwapTilesetSystem(Entity& player, Camera& camera, Grid& grid) {
    OPTIONS.is_ascii = !OPTIONS.is_ascii;
    SpritesheetLoadingSystem();
    SPRITE_REGISTRY.clear();
    SpriteGenerator();
    grid.reloadSprites();
    for (auto& [entity, renderable] : renderables) {
        renderable.sprite = *getSpriteTile(renderable.sprite_type);
    }
    ResizeSystem(player, camera, grid);
    // grid.updateMap();
}
