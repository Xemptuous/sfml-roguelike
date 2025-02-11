#include "engine.hpp"

#include "entity.hpp"
#include "grid.hpp"
#include "sprite.hpp"
#include "ui.hpp"

#include <SFML/Window/Window.hpp>
#include <algorithm>
#include <cmath>
#include <memory>

extern const int MAP_WIDTH, MAP_HEIGHT;
extern const int SPRITE_WIDTH, SPRITE_HEIGHT;
extern const int RENDER_WIDTH, RENDER_HEIGHT;
extern const int CONSOLE_WIDTH, CONSOLE_HEIGHT;
extern Options OPTIONS;

MenuOptions MENU_OPTIONS = {.show_inventory = false};

void DrawSystem(
    RenderWindow& window, RenderTexture& render, Font& font, Camera& camera, Grid& grid, ECS& ecs
) {
    window.clear();
    render.clear();
    render.setView(camera.view);
    camera.view.setViewport({
        {0.f, 0.f },
        {1.f, 0.8f}
    });
    RenderSystem(render, camera, grid, ecs);
    render.display();
    window.draw(sf::Sprite(render.getTexture()));
    UISystem(window, font, ecs);
    // if (MENU_OPTIONS.show_inventory) showInventory(window, font, ecs);
    if (MENU_OPTIONS.show_inventory) RenderInventory(window, font, ecs);
    window.display();
}

void RenderSystem(RenderTexture& renderTexture, Camera& camera, Grid& grid, ECS& ecs) {
    // draw the map
    for (int x = camera.x1; x < camera.x2; x++) {
        for (int y = camera.y1; y < camera.y2; y++) {
            int idx = xy_idx(x, y);

            std::shared_ptr<Tile> tile = grid.tiles[idx];

            Sprite* bg = getSpriteTile(OPTIONS.is_ascii ? BrownWall1 : Block);
            renderTexture.draw(*bg);
            // if (OPTIONS.is_ascii) {
            //     renderTexture.draw(tile.bg_sprite);
            // }
            renderTexture.draw(*tile->sprite);
        }
    }

    // draw entities
    std::vector<std::pair<Entity, Renderable>> renderQueue;
    for (Entity entity : ecs.entities()) {
        if (!ecs.has_component<Renderable>(entity)) continue;
        Renderable* renderable = ecs.get_component<Renderable>(entity);
        renderQueue.push_back({entity, *renderable});
    }
    std::sort(renderQueue.begin(), renderQueue.end(), [](auto& a, auto& b) {
        return a.second.zIndex < b.second.zIndex;
    });

    for (auto& [entity, renderable] : renderQueue) {
        if (!ecs.has_component<Position>(entity)) continue;
        const Position* pos = ecs.get_component<Position>(entity);
        if (!pos) continue;

        Vector2f posv{
            std::round(OPTIONS.SIZE_FACTOR.x * pos->x),
            std::round(OPTIONS.SIZE_FACTOR.y * pos->y),
        };
        renderable.sprite.setPosition(posv);
        renderable.sprite.setScale(OPTIONS.SCALE_FACTOR);
        Sprite* bg = getSpriteTile(OPTIONS.is_ascii ? BrownWall1 : Block);
        bg->setPosition(posv);
        bg->setScale(OPTIONS.SCALE_FACTOR);
        bg->setColor(sf::Color::Black);
        renderTexture.draw(*bg);
        renderTexture.draw(renderable.sprite);
    }
}

void InputSystem(ECS& ecs) {
    using namespace sf::Keyboard;
    int dx = 0, dy = 0;
    // Orthogonol Movement
    if (isKeyPressed(Key::Left) || isKeyPressed(Key::H)) dx = -1;
    if (isKeyPressed(Key::Right) || isKeyPressed(Key::L)) dx = 1;
    if (isKeyPressed(Key::Up) || isKeyPressed(Key::K)) dy = -1;
    if (isKeyPressed(Key::Down) || isKeyPressed(Key::J)) dy = 1;
    // Diagonal Movement
    if (isKeyPressed(Key::Y)) {
        dx = -1;
        dy = -1;
    }
    if (isKeyPressed(Key::U)) {
        dx = 1;
        dy = -1;
    }
    if (isKeyPressed(Key::B)) {
        dx = -1;
        dy = 1;
    }
    if (isKeyPressed(Key::N)) {
        dx = 1;
        dy = 1;
    }

    if (dx != 0 || dy != 0) {
        Movement* mov = ecs.get_component<Movement>(Player);

        mov->dx = dx;
        mov->dy = dy;
    }

    // if (isKeyPressed(Key::K)) tryMoveView(1, -1);
    // if (isKeyPressed(Key::H)) tryMoveView(-1, 0);
    // if (isKeyPressed(Key::J)) tryMoveView(0, 1);
    // if (isKeyPressed(Key::L)) tryMoveView(1, 0);
}

void CameraSystem(Camera& camera, ECS& ecs) {
    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);

    bool can_move_view_x = true;
    bool can_move_view_y = true;

    int view_cw = (camera.x2 - camera.x1) / 2;
    int view_ch = (camera.y2 - camera.y1) / 2;

    bool view_at_left   = camera.x1 <= 0;
    bool view_at_right  = camera.x2 >= MAP_WIDTH;
    bool view_at_top    = camera.y1 <= 0;
    bool view_at_bottom = camera.y2 >= MAP_HEIGHT;

    bool player_left_of_center  = playerPos->x <= camera.x2 / 2;
    bool player_right_of_center = playerPos->x >= MAP_WIDTH - view_cw;
    bool player_above_center    = playerPos->y <= camera.y2 / 2;
    bool player_below_center    = playerPos->y >= MAP_HEIGHT - view_ch;

    if (playerMov->dx < 0 && (view_at_left || (view_at_right && player_right_of_center)))
        can_move_view_x = false;
    else if (playerMov->dx > 0 && (view_at_right || (view_at_left && player_left_of_center)))
        can_move_view_x = false;
    else if (playerMov->dy < 0 && (view_at_top || (view_at_bottom && player_below_center)))
        can_move_view_y = false;
    else if (playerMov->dy > 0 && (view_at_bottom || (view_at_top && player_above_center)))
        can_move_view_y = false;

    if (can_move_view_x) {
        camera.moveCamera(playerMov->dx, 0);
        ResizeCameraSystem(camera);
    };

    if (can_move_view_y) {
        camera.moveCamera(0, playerMov->dy);
        ResizeCameraSystem(camera);
    };
}

void ResizeSystem(Camera& camera, Grid& grid, ECS& ecs) {
    // update scale and size factors
    OPTIONS.SIZE_FACTOR = Vector2f{
        (float)RENDER_WIDTH / CONSOLE_WIDTH,
        (float)RENDER_HEIGHT / CONSOLE_HEIGHT,
    };
    OPTIONS.SCALE_FACTOR = Vector2f{
        OPTIONS.SIZE_FACTOR.x / SPRITE_WIDTH,
        OPTIONS.SIZE_FACTOR.y / SPRITE_HEIGHT,
    };

    // update camera position
    Position* playerPos = ecs.get_component<Position>(Player);
    camera.view.setCenter({
        playerPos->x * OPTIONS.SIZE_FACTOR.x,
        playerPos->y * OPTIONS.SIZE_FACTOR.y - CONSOLE_HEIGHT,
    });
    camera.view.setSize(Vector2f{
        CONSOLE_WIDTH * OPTIONS.SIZE_FACTOR.x,
        CONSOLE_HEIGHT * OPTIONS.SIZE_FACTOR.y,
    });

    // update camera dimensions
    ResizeCameraSystem(camera);

    // resize entities
    for (Entity entity : ecs.entities()) {
        if (!ecs.has_component<Renderable>(entity)) continue;
        Vector2f pos{
            std::round(OPTIONS.SIZE_FACTOR.x * playerPos->x),
            std::round(OPTIONS.SIZE_FACTOR.y * playerPos->y),
        };
        Renderable* renderable = ecs.get_component<Renderable>(entity);
        if (!renderable) continue;
        renderable->sprite.setPosition(pos);
        renderable->sprite.setScale(OPTIONS.SCALE_FACTOR);
        if (OPTIONS.is_ascii) {
            renderable->sprite.setColor(renderable->fg);
        }
    }

    // resize map tiles
    for (std::shared_ptr<Tile> tile : grid.tiles) {
        Vector2f pos{
            std::round(OPTIONS.SIZE_FACTOR.x * tile->position.x),
            std::round(OPTIONS.SIZE_FACTOR.y * tile->position.y),
        };
        tile->sprite->setPosition(pos);
        tile->sprite->setScale(OPTIONS.SCALE_FACTOR);
        if (OPTIONS.is_ascii) {
            // tile->bg_sprite.setPosition(pos);
            // tile->bg_sprite.setScale(OPTIONS.SCALE_FACTOR);
            // tile->bg_sprite.setColor(tile.bg);
            tile->sprite->setColor(tile->fg);
        }
    }
}

void ResizeCameraSystem(Camera& camera) {
    Vector2f size   = camera.view.getSize();
    Vector2f center = camera.view.getCenter();

    int cx = center.x / OPTIONS.SIZE_FACTOR.x;
    int cy = center.y / OPTIONS.SIZE_FACTOR.y;
    int sx = size.x / OPTIONS.SIZE_FACTOR.x;
    int sy = size.y / OPTIONS.SIZE_FACTOR.y;

    int x1 = std::max(0, cx - sx / 2);
    int y1 = std::max(0, cy - sy / 2);

    int x2 = std::min(MAP_WIDTH, x1 + sx);
    int y2 = std::min(MAP_HEIGHT, y1 + sy);

    camera.x1 = x1;
    camera.x2 = x2;
    camera.y1 = y1;
    camera.y2 = y2;
};

void SwapTilesetSystem(Camera& camera, Grid& grid, ECS& ecs) {
    OPTIONS.is_ascii = !OPTIONS.is_ascii;
    SpritesheetLoadingSystem();
    SPRITE_REGISTRY.clear();
    SpriteGenerator();
    grid.reloadSprites();
    for (Entity entity : ecs.entities()) {
        if (!ecs.has_component<Renderable>(entity)) continue;
        Renderable* renderable = ecs.get_component<Renderable>(entity);
        renderable->sprite     = *getSpriteTile(renderable->sprite_type);
    }
    ResizeSystem(camera, grid, ecs);
    // grid.updateMap();
}

void Camera::moveCamera(int dx, int dy) {
    int nx1 = x1 + dx;
    int nx2 = x2 + dx;
    int ny1 = y1 + dy;
    int ny2 = y2 + dy;

    if (nx1 < 0 || ny1 < 0 || nx2 > MAP_WIDTH || ny2 > MAP_HEIGHT) {
        return;
    }
    this->view.move({
        dx * SPRITE_WIDTH * OPTIONS.SCALE_FACTOR.x,
        dy * SPRITE_HEIGHT * OPTIONS.SCALE_FACTOR.y,
    });
}
