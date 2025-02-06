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
