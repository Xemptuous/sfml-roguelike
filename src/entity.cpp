#include "entity.hpp"

#include "color.hpp"

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
            Renderable render;
            Health health;
            Damage damage;
            switch (picker(rng)) {
                case 0:
                    render = Renderable(GoblinUnarmed, getColor(Green));
                    health = {10, 10};
                    damage = {1, 1};
                    break;
                case 1:
                    render = Renderable(GoblinSword, getColor(Green));
                    health = {10, 10};
                    damage = {2, 3};
                    break;
                case 2:
                    render = Renderable(GoblinBow, getColor(Green));
                    health = {10, 10};
                    damage = {3, 4};
                    break;
                case 3:
                    render = Renderable(BarbarianUnarmed, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {2, 2};
                    break;
                case 4:
                    render = Renderable(BarbarianSword, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {3, 4};
                    break;
                case 5:
                    render = Renderable(BarbarianBow, getColor(SandyBrown));
                    health = {15, 10};
                    damage = {4, 4};
                    break;
                case 6:
                    render = Renderable(ReptileUnarmed, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {2, 2};
                    break;
                case 7:
                    render = Renderable(ReptileSword, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {3, 4};
                    break;
                case 8:
                    render = Renderable(ReptileBow, getColor(DarkGreen));
                    health = {20, 10};
                    damage = {4, 4};
                    break;
                case 9:
                    render = Renderable(DemonUnarmed, getColor(Red));
                    health = {50, 10};
                    damage = {4, 4};
                    break;
                default:
                    render = Renderable(DemonAxe, getColor(Red));
                    health = {50, 10};
                    damage = {7, 7};
                    break;
            }
            ecs.add_component(entity, render);
            ecs.add_component(entity, health);
            ecs.add_component(entity, damage);
        }
        ecs.add_component(entity, AIBehavior::Standard);
    }
}

void AIMovementSystem(ECS& ecs) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> std_mov(-1, 1);
    std::uniform_int_distribution<int> agg_mov(-2, 2);
    for (Entity entity : ecs.entities()) {
        AIBehavior* behavior = ecs.get_component<AIBehavior>(entity);
        if (behavior == nullptr) {
            continue;
        }
        Movement* mov = ecs.get_component<Movement>(entity);
        switch (*behavior) {
            case Standard:
                mov->dx = std_mov(rng);
                mov->dy = std_mov(rng);
                break;
            case Aggressive:
                mov->dx = std_mov(rng);
                mov->dy = std_mov(rng);
                break;
            default: break;
        }
    }
}
