#pragma once
#include "ecs.hpp"
#include "grid.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
using namespace sf;

struct MenuOptions {
    bool show_inventory;
};
struct Options {
    bool is_ascii;
    Vector2f SCALE_FACTOR, SIZE_FACTOR;
};

extern MenuOptions MENU_OPTIONS;
extern Options OPTIONS;

struct Camera {
    sf::View gameView;
    sf::View uiView;
    int x1; // Top left
    int y1; // Top Left
    int x2; // Bottom Right
    int y2; // Bottom Right

    void moveCamera(int dx, int dy);
};

// Systems
void DrawSystem(RenderWindow&, Font&, Camera&, Grid&, ECS&);
void UISystem(RenderWindow&, Font&, Camera&, ECS&);
void RenderSystem(RenderWindow&, Camera&, Grid&, ECS&);
void CameraSystem(Camera&, ECS&);

void InputSystem(ECS&);

void ResizeSystem(Camera&, Grid&, ECS&);
void ResizeCameraSystem(Camera&);

void SwapTilesetSystem(Camera&, Grid&, ECS&);
