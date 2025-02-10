#pragma once
#include <cstdint>
#include <queue>
#include <unordered_set>

using Entity = uint64_t;

struct EntityManager {
    Entity next_entity_id = 0;
    std::queue<Entity> recycled_ids;
    std::unordered_set<Entity> active_entities;

    EntityManager() = default;

    Entity create_entity() {
        Entity id;
        if (!recycled_ids.empty()) {
            id = recycled_ids.front();
            recycled_ids.pop();
        } else {
            id = next_entity_id++;
        }
        active_entities.insert(id);
        return id;
    };

    void destroy_entity(Entity entity) {
        active_entities.erase(entity);
        recycled_ids.push(entity);
        // You would also remove associated components here
    };

    bool is_active(Entity entity) const {
        return active_entities.find(entity) != active_entities.end();
    }
};
