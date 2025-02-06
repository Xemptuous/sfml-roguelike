#include "entity.hpp"

#include <iostream>
#include <random>

extern const int MAP_WIDTH, MAP_HEIGHT;
const uint16_t NUM_ENTITIES = 100;

// extern std::unordered_map<Entity, Position> positions;
// extern std::unordered_map<Entity, Renderable> renderables;
// std::unordered_map<Entity, Health> healths;

enum AIBehavior {
    Standard,
    Aggressive,
};

void MovementSystem(ECS& ecs) {
    for (Entity entity : ecs.entities()) {
        Position* pos = ecs.get_component<Position>(entity);
        Movement* mov = ecs.get_component<Movement>(entity);

        pos->x += mov->dx;
        pos->y += mov->dy;
    }
}

void CollisionSystem(Grid& grid, ECS& ecs) {
    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);
    for (Entity entity : ecs.entities()) {
        Position* pos = ecs.get_component<Position>(entity);
        Movement* mov = ecs.get_component<Movement>(entity);
        int dest_x    = pos->x;
        int dest_y    = pos->y;

        if (!grid.isWalkable(dest_x, dest_y)) {
            // undo movement if colliding
            pos->x -= mov->dx;
            pos->y -= mov->dy;

            mov->dx = 0;
            mov->dy = 0;
        }
    }
}

void AIMovementSystem(ECS& ecs) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> std_mov(-1, 1);
    std::uniform_int_distribution<int> agg_mov(-2, 2);

    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);
    int playerX         = playerPos->x + playerMov->dx;
    int playerY         = playerPos->y + playerMov->dy;

    for (Entity entity : ecs.entities()) {
        if (entity == Player) continue;

        Movement* mov = ecs.get_component<Movement>(entity);
        Position* pos = ecs.get_component<Position>(entity);

        switch (*ecs.get_component<AIBehavior>(entity)) {
            case Standard:
                mov->dx = std_mov(rng);
                mov->dy = std_mov(rng);
                break;
            case Aggressive: {
                Vision vision    = *ecs.get_component<Vision>(entity);
                bool playerFound = false;

                // search for player somewhere within this entity's vision range
                // TODO: include object collision in LOS (e.g. walls)
                for (int y = pos->y - vision; y < pos->y + vision; y++) {
                    for (int x = pos->x - vision; x < pos->x + vision; x++) {
                        if (x == playerX && y == playerY) {
                            playerFound = true;
                            break;
                        }
                    }
                    if (playerFound) break;
                }

                // move towards the player
                if (playerFound) {
                    mov->dx = playerX > pos->x ? 1 : playerX < pos->x ? -1 : 0;
                    mov->dy = playerY > pos->y ? 1 : playerY < pos->y ? -1 : 0;
                }
                // randomly wander
                else {
                    mov->dx = std_mov(rng);
                    mov->dy = std_mov(rng);
                }
                break;
            }
            default: break;
        }
    }
}

void CombatSystem(ECS& ecs) {
    Position* playerPos = ecs.get_component<Position>(Player);
    Movement* playerMov = ecs.get_component<Movement>(Player);

    for (Entity entity : ecs.entities()) {
        if (entity == Player) continue;
        Movement* mov = ecs.get_component<Movement>(entity);
        Position* pos = ecs.get_component<Position>(entity);

        // if moving into the player, do combat
        if (pos->x == playerPos->x && pos->y == playerPos->y) {
            Name* attName  = ecs.get_component<Name>(entity);
            Name* defName  = ecs.get_component<Name>(Player);
            Damage* damage = ecs.get_component<Damage>(entity);
            Health* health = ecs.get_component<Health>(Player);

            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<int> rand_dmg(damage->min, damage->max);
            int dmg = rand_dmg(rng);

            health->curr -= dmg;
            std::cout << *attName << " attacks " << *defName << " for " << dmg << '\n';
            std::cout << *defName << " health: " << health->curr << "/" << health->max << '\n';

            pos->x -= mov->dx;
            pos->y -= mov->dy;
        }
    }
}

void EntityGeneratorSystem(ECS& ecs) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> picker(0, 10);
    std::uniform_int_distribution<int> randx(0, MAP_WIDTH);
    std::uniform_int_distribution<int> randy(0, MAP_HEIGHT);

    using namespace rl;
    for (int i = 0; i < NUM_ENTITIES; i++) {
        Entity entity = ecs.create_entity();
        ecs.add_component(entity, Position{randx(rng), randy(rng)});
        ecs.add_component(entity, Movement{0, 0});
        {
            Name name;
            Renderable render;
            Health health;
            Damage damage;
            Vision vision;
            switch (picker(rng)) {
                case 0:
                    name   = "Goblin";
                    render = Renderable(GoblinUnarmed, getColor(Green));
                    health = {10, 10};
                    damage = {1, 1};
                    vision = 5;
                    break;
                case 1:
                    name   = "Goblin";
                    render = Renderable(GoblinSword, getColor(Green));
                    health = {10, 10};
                    damage = {2, 3};
                    vision = 5;
                    break;
                case 2:
                    name   = "Goblin";
                    render = Renderable(GoblinBow, getColor(Green));
                    health = {10, 10};
                    damage = {3, 4};
                    vision = 5;
                    break;
                case 3:
                    name   = "Barbarian";
                    render = Renderable(BarbarianUnarmed, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {2, 2};
                    vision = 5;
                    break;
                case 4:
                    name   = "Barbarian";
                    render = Renderable(BarbarianSword, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {3, 4};
                    vision = 5;
                    break;
                case 5:
                    name   = "Barbarian";
                    render = Renderable(BarbarianBow, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {4, 4};
                    vision = 5;
                    break;
                case 6:
                    name   = "Reptile";
                    render = Renderable(ReptileUnarmed, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {2, 2};
                    vision = 5;
                    break;
                case 7:
                    name   = "Reptile";
                    render = Renderable(ReptileSword, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {3, 4};
                    vision = 5;
                    break;
                case 8:
                    name   = "Reptile";
                    render = Renderable(ReptileBow, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {4, 4};
                    vision = 5;
                    break;
                case 9:
                    name   = "Demon";
                    render = Renderable(DemonUnarmed, getColor(Red));
                    health = {50, 10};
                    damage = {4, 4};
                    vision = 5;
                    break;
                default:
                    name   = "Demon";
                    render = Renderable(DemonAxe, getColor(Red));
                    health = {50, 10};
                    damage = {7, 7};
                    vision = 5;
                    break;
            }
            ecs.add_component(entity, name);
            ecs.add_component(entity, render);
            ecs.add_component(entity, health);
            ecs.add_component(entity, damage);
            ecs.add_component(entity, vision);
        }
        ecs.add_component(entity, AIBehavior::Aggressive);
    }
}
