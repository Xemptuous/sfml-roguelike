#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <unordered_set>

using namespace sf;

using Entity = std::uint64_t;

const Entity MAX_ENTITIES = 10000;

// Components
struct Health {
    int curr, max;
};

struct PlayerControlled {};
struct AIControlled {};

struct EntityManager {
    Entity next_entity_id = 0;
    std::queue<Entity> recycled_ids;
    std::unordered_set<Entity> active_entities;

    Entity create_entity();
    void destroy_entity(Entity);
    bool is_active(Entity entity) const {
        return active_entities.find(entity) != active_entities.end();
    }
};

// struct Player {
//     Sprite sprite;
//     SpriteTiles sprite_type;
//     Vector2i position;
//
//     Player(SpriteTiles tile);
// };

// struct Monster {
//     Sprite* sprite;
//     Vector2i position;
// };

// Systems
void EntityGeneratorSystem(EntityManager&);
// void HealthSystem();
