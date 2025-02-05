#pragma once
#include "ecs.hpp"
#include "grid.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>

struct Options {
    bool is_ascii;
};

struct Movement {
    int dx, dy;
};

struct Camera {
    sf::View view;
    int x1; // Top left
    int y1; // Top Left
    int x2; // Bottom Right
    int y2; // Bottom Right
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

// Systems
void MovementSystem(ECS&);

void DrawSystem(RenderWindow&, RenderTexture&, Camera&, Grid&, ECS&);
void RenderSystem(RenderTexture&, Camera&, Grid&, ECS&);
void CameraSystem(Entity& player, Camera&, ECS&);

void InputSystem(Entity& player, ECS&);
void CollisionSystem(Grid&, ECS&);

void ResizeSystem(Entity& player, Camera&, Grid&, ECS&);
void ResizeCameraSystem(Camera&);

void SwapTilesetSystem(Entity& player, Camera&, Grid&, ECS&);
