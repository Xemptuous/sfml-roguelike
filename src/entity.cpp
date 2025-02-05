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
        Renderable renderable;
        switch (picker(rng)) {
            case 0:  renderable = Renderable(GoblinUnarmed, getColor(Green)); break;
            case 1:  renderable = Renderable(GoblinSword, getColor(Green)); break;
            case 2:  renderable = Renderable(GoblinBow, getColor(Green)); break;
            case 3:  renderable = Renderable(BarbarianUnarmed, getColor(SandyBrown)); break;
            case 4:  renderable = Renderable(BarbarianSword, getColor(SandyBrown)); break;
            case 5:  renderable = Renderable(BarbarianBow, getColor(SandyBrown)); break;
            case 6:  renderable = Renderable(ReptileUnarmed, getColor(DarkGreen)); break;
            case 7:  renderable = Renderable(ReptileSword, getColor(DarkGreen)); break;
            case 8:  renderable = Renderable(ReptileBow, getColor(DarkGreen)); break;
            case 9:  renderable = Renderable(DemonUnarmed, getColor(Red)); break;
            default: renderable = Renderable(DemonAxe, getColor(Red)); break;
        }
        ecs.add_component(entity, renderable);
        ecs.add_component(entity, AIBehavior::Standard);
    }
}

void AIBehaviorSystem(ECS& ecs) {
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
