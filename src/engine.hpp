#pragma once
#include "ecs.hpp"
#include "grid.hpp"

#include <SFML/Graphics.hpp>

using namespace sf;

struct Options {
    bool is_ascii;
};

struct Camera {
    sf::View view;
    int x1; // Top left
    int y1; // Top Left
    int x2; // Bottom Right
    int y2; // Bottom Right
};

// Systems
void DrawSystem(RenderWindow&, RenderTexture&, Camera&, Grid&, ECS&);
void RenderSystem(RenderTexture&, Camera&, Grid&, ECS&);
void CameraSystem(Entity& player, Camera&, ECS&);

void InputSystem(Entity& player, ECS&);
void MovementSystem(ECS&);
void CollisionSystem(Grid&, ECS&);

void ResizeSystem(Entity& player, Camera&, Grid&, ECS&);
void ResizeCameraSystem(Camera&);

void SwapTilesetSystem(Entity& player, Camera&, Grid&, ECS&);
