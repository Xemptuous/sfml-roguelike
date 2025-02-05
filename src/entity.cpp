#include "entity.hpp"

#include "engine.hpp"
#include "grid.hpp"
#include "sprite.hpp"

#include <cmath>

extern const int MAP_WIDTH, MAP_HEIGHT;

extern std::unordered_map<Entity, Position> positions;
extern std::unordered_map<Entity, Renderable> renderables;
std::unordered_map<Entity, Health> healths;

// Player::Player(SpriteTiles tile)
//     : sprite(*getSpriteTile(tile)), sprite_type(tile), position({MAP_WIDTH / 2, MAP_HEIGHT / 2})
//     {}

void EntityGeneratorSystem(EntityManager& em) {
    // // create Player
    // Entity player       = em.create_entity();
    // positions[player]   = {MAP_WIDTH / 2, MAP_HEIGHT / 2};
    // renderables[player] = Renderable(PlayerMaleStanding);

    // create entities
}

Entity EntityManager::create_entity() {
    Entity id;
    if (!recycled_ids.empty()) {
        id = recycled_ids.front();
        recycled_ids.pop();
    } else {
        id = next_entity_id++;
    }
    active_entities.insert(id);
    return id;
}

void EntityManager::destroy_entity(Entity entity) {
    active_entities.erase(entity);
    recycled_ids.push(entity);
    // You would also remove associated components here
}
