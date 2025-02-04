#include "entity.hpp"
#include "grid.hpp"
#include "sprite.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>

// Components
struct Options {
    bool is_ascii;
};

struct Renderable {
    sf::Sprite sprite;
    SpriteTiles sprite_type;
    sf::Color fg;
    sf::Color bg;
    int zIndex;

    Renderable()
        : sprite(*getSpriteTile(None)),
          sprite_type(None),
          fg(sf::Color::White),
          bg(sf::Color::Black),
          zIndex(1) {}
    Renderable(SpriteTiles stype)
        : sprite(*getSpriteTile(stype)),
          sprite_type(stype),
          fg(sf::Color::White),
          bg(sf::Color::Black),
          zIndex(1) {}
    Renderable(SpriteTiles stype, sf::Color fg)
        : sprite(*getSpriteTile(stype)),
          sprite_type(stype),
          fg(fg),
          bg(sf::Color::Black),
          zIndex(1) {}
    Renderable(SpriteTiles stype, sf::Color fg, sf::Color bg)
        : sprite(*getSpriteTile(stype)), sprite_type(stype), fg(fg), bg(bg), zIndex(1) {}
    Renderable(SpriteTiles stype, sf::Color fg, sf::Color bg, int z)
        : sprite(*getSpriteTile(stype)), sprite_type(stype), fg(fg), bg(bg), zIndex(z) {}
    Renderable(SpriteTiles stype, sf::Color fg, int z)
        : sprite(*getSpriteTile(stype)),
          sprite_type(stype),
          fg(fg),
          bg(sf::Color::Black),
          zIndex(z) {}
    Renderable(SpriteTiles stype, int z)
        : sprite(*getSpriteTile(stype)),
          sprite_type(stype),
          fg(sf::Color::White),
          bg(sf::Color::Black),
          zIndex(z) {}
};

struct Camera {
    sf::View view;
    int x1; // Top left
    int y1; // Top Left
    int x2; // Bottom Right
    int y2; // Bottom Right
};

struct Movement {
    int dx, dy;
};

// Systems
void MovementSystem(std::vector<Entity>& entities);

void DrawSystem(RenderWindow& window, RenderTexture& renderTexture, Camera& camera, Grid& grid);
void RenderSystem(RenderTexture& renderTexture, Camera& camera, Grid& grid);
void CameraSystem(Entity& player, Camera& camera);

void InputSystem(Entity& player);
void CollisionSystem(Grid& grid);

void ResizeSystem(Entity& player, Camera& camera, Grid& grid);
void ResizeCameraSystem(Camera& camera);

void SwapTilesetSystem(Entity& player, Camera& camera, Grid& grid);
