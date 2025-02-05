#include "entity.hpp"

extern const int MAP_WIDTH, MAP_HEIGHT;

// extern std::unordered_map<Entity, Position> positions;
// extern std::unordered_map<Entity, Renderable> renderables;
// std::unordered_map<Entity, Health> healths;

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
