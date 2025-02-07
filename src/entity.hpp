#pragma once
#include "ecs.hpp"
#include "grid.hpp"
#include "sprite.hpp"

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

using Entity = uint64_t;

const Entity MAX_ENTITIES = 10000;

static const Entity Player = 0;

// Components
typedef std::string Name;
struct Position {
    int x, y;
};
struct Movement {
    int dx, dy;
};
typedef int Vision;
struct Health {
    int curr, max;
};
struct Damage {
    int min, max;
};

struct Renderable {
    sf::Sprite sprite;
    SpriteTiles sprite_type;
    sf::Color fg;
    sf::Color bg;
    int zIndex;

    Renderable() :
        sprite(*getSpriteTile(None)),
        sprite_type(None),

        fg(sf::Color::White),
        bg(sf::Color::Black),
        zIndex(1) {}
    Renderable(SpriteTiles stype) :
        sprite(*getSpriteTile(stype)),
        sprite_type(stype),
        fg(sf::Color::White),
        bg(sf::Color::Black),
        zIndex(1) {}
    Renderable(SpriteTiles stype, sf::Color fg) :
        sprite(*getSpriteTile(stype)),
        sprite_type(stype),
        fg(fg),
        bg(sf::Color::Black),
        zIndex(1) {}
    Renderable(SpriteTiles stype, sf::Color fg, sf::Color bg) :
        sprite(*getSpriteTile(stype)), sprite_type(stype), fg(fg), bg(bg), zIndex(1) {}
    Renderable(SpriteTiles stype, sf::Color fg, sf::Color bg, int z) :
        sprite(*getSpriteTile(stype)), sprite_type(stype), fg(fg), bg(bg), zIndex(z) {}
    Renderable(SpriteTiles stype, sf::Color fg, int z) :
        sprite(*getSpriteTile(stype)),
        sprite_type(stype),
        fg(fg),
        bg(sf::Color::Black),
        zIndex(z) {}
    Renderable(SpriteTiles stype, int z) :
        sprite(*getSpriteTile(stype)),
        sprite_type(stype),
        fg(sf::Color::White),
        bg(sf::Color::Black),
        zIndex(z) {}
};

void MovementSystem(ECS&);
void AIMovementSystem(Grid&, ECS&);
void CollisionSystem(Grid&, ECS&);
void LineOfSightSystem(ECS&);
void PathFindingSystem(Entity start, Entity end, Grid&, ECS&);
void CombatSystem(ECS&);

void EntityGeneratorSystem(ECS&);
