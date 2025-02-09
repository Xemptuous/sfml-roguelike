#pragma once
#include "ecs.hpp"
#include "grid.hpp"

struct MenuOptions {
    bool show_inventory;
};

extern MenuOptions MENU_OPTIONS;
// extern bool show_inventory;

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

    void moveCamera(int dx, int dy);
};

// Systems
void DrawSystem(RenderWindow&, RenderTexture&, Font&, Camera&, Grid&, ECS&);
void UISystem(RenderWindow&, Font&, Camera&, ECS&);
void RenderSystem(RenderTexture&, Camera&, Grid&, ECS&);
void CameraSystem(Camera&, ECS&);

void InputSystem(ECS&);

void ResizeSystem(Camera&, Grid&, ECS&);
void ResizeCameraSystem(Camera&);

void SwapTilesetSystem(Camera&, Grid&, ECS&);
